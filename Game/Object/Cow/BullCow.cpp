#include "BullCow.h"

BullCow::BullCow() { Init(); }

BullCow::~BullCow() { Finalize(); }

void BullCow::Finalize() {

}

void BullCow::Init() {

	// ワールド空間での中心座標
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::BULL) {
				worldPos_ = {
					col * tileSize_.x + (tileSize_.x * 0.5f),
					row * tileSize_.y + (tileSize_.y * 0.5f)
				};
			}

			isDisplay_ = true;
		}
	}

	// csvで方向の評価
	cannotMoveGrid_ = LoadFile("./Resources/cow/bullEightDireRange.csv");

	// ローカルアドレスでのbullの位置を保存
	for (int row = 0; row < cannotMoveGrid_.size(); row++) {
		for (int col = 0; col < cannotMoveGrid_[0].size(); col++) {
			switch (cannotMoveGrid_[row][col]) {
			case kCanMoveDirection::top:
				direAddressNum_[kCanMoveDirection::top]++;
				break;

			case kCanMoveDirection::bottom:
				direAddressNum_[kCanMoveDirection::bottom]++;
				break;

			case kCanMoveDirection::left:
				direAddressNum_[kCanMoveDirection::left]++;
				break;

			case kCanMoveDirection::right:
				direAddressNum_[kCanMoveDirection::right]++;
				break;

			case kCanMoveDirection::leftTop:
				direAddressNum_[kCanMoveDirection::leftTop]++;
				break;

			case kCanMoveDirection::rightTop:
				direAddressNum_[kCanMoveDirection::rightTop]++;
				break;

			case kCanMoveDirection::leftBottom:
				direAddressNum_[kCanMoveDirection::leftBottom]++;
				break;

			case kCanMoveDirection::rightBottom:
				direAddressNum_[kCanMoveDirection::rightBottom]++;
				break;

			case 50:
				localCenterAdd_.x = col;
				localCenterAdd_.y = row;
				break;
			}
		}
	}

	for (int i = 0; i < 8; i++) {
		cannotMove_[i].localAdd.resize(direAddressNum_[i]);
		cannotMove_[i].worldAdd.resize(direAddressNum_[i]);

		moveAreas_[i].worldPos.resize(direAddressNum_[i]);

		moveAreas_[i].screenVertex.resize(direAddressNum_[i]);

		moveAreas_[i].screenMatrix.resize(direAddressNum_[i]);
		moveAreas_[i].worldMatrix.resize(direAddressNum_[i]);
	}

	gh_ = Novice::LoadTexture("white1x1.png");

	// 移動状態
	isIdle_ = false;

	isMove_ = false;

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ -tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
	};

	MakeWorldMatrix();

	CenterAddUpdate();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	moveScalar_.x = 2;
	moveScalar_.y = 2;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		nearWallOfValue_[i] = 0;
	}

	// 評価した数値の最大値
	maxDireValue_ = 0;
	// その添え字
	maxDireValueIndex_ = 0;

	// 全方向囲まれている時
	adjoinNum_ = 0;

	// 最終的に動く方向
	movedDire_ = top;

	//=========================================
	// 評価値(ここの数値をいじればいける)
	value_.wall = 5;
	value_.fourArea = 12;
	value_.allDire = 10;
	value_.clamp = 2;

	value_.dog = 150;
	value_.rock = 1000;
	value_.adjoin = 15;
	value_.adjoinAdd = 20;
	value_.fence = 200;
	value_.slantFence = -200;

	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	isFenceAttack_ = false;

	//=========================================
	//　移動で使う
	startPos_ = { 0.0f, 0.0f };
	endPos_ = { 0.0f, 0.0f };

	easeT_ = 0.0f;
	frameCount_ = 0.0f;
	frameCountLimit_ = 15.0f;

	moveGridNum_ = 2;

}

void BullCow::Update() {

	// 牛の現在の位置を取得
	CenterAddUpdate();

	MoveTurn();

	CheckMoveDire();

	// 動く
	Move();

	// 移動後のアドレスを計算する
	CenterAddUpdate();

	MoveAreasUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

}

void BullCow::Draw() {
	if (isDisplay_) {
		Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFF0000FF);
	}

	for (int dire = 0; dire < 8; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			Draw::Quad(moveAreas_[dire].screenVertex[i], { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFF000044);
		}
	}

	ImguiDraw();
}

void BullCow::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

	for (int dire = 0; dire < 8; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			moveAreas_[dire].screenMatrix[i] = MakeWvpVpMatrix(moveAreas_[dire].worldMatrix[i], viewMatrix, orthoMatrix, viewportMatrix);

			moveAreas_[dire].screenVertex[i].lt = Transform(localVertex_.lt, moveAreas_[dire].screenMatrix[i]);
			moveAreas_[dire].screenVertex[i].rt = Transform(localVertex_.rt, moveAreas_[dire].screenMatrix[i]);
			moveAreas_[dire].screenVertex[i].lb = Transform(localVertex_.lb, moveAreas_[dire].screenMatrix[i]);
			moveAreas_[dire].screenVertex[i].rb = Transform(localVertex_.rb, moveAreas_[dire].screenMatrix[i]);
		}
	}

}

void BullCow::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

	for (int dire = 0; dire < 8; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			moveAreas_[dire].worldMatrix[i] = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, moveAreas_[dire].worldPos[i]);
		}
	}

}

// ------ 今のアドレスの計算 ------ //
void BullCow::CenterAddUpdate() {
	worldAdd_ = {
		static_cast<int>(worldPos_.x / tileSize_.x),
		static_cast<int>(worldPos_.y / tileSize_.y)
	};

	int index[8] = { 0 };

	for (int row = 0; row < cannotMoveGrid_.size(); row++) {
		for (int col = 0; col < cannotMoveGrid_[0].size(); col++) {
			switch (cannotMoveGrid_[row][col]) {
			case kCanMoveDirection::top:
				cannotMove_[kCanMoveDirection::top].localAdd[index[0]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[0]++;
				break;

			case  kCanMoveDirection::bottom:
				cannotMove_[kCanMoveDirection::bottom].localAdd[index[1]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[1]++;
				break;

			case  kCanMoveDirection::left:
				cannotMove_[kCanMoveDirection::left].localAdd[index[2]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[2]++;
				break;

			case  kCanMoveDirection::right:
				cannotMove_[kCanMoveDirection::right].localAdd[index[3]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[3]++;
				break;

			case  kCanMoveDirection::leftTop:
				cannotMove_[kCanMoveDirection::leftTop].localAdd[index[4]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[4]++;
				break;

			case  kCanMoveDirection::rightTop:
				cannotMove_[kCanMoveDirection::rightTop].localAdd[index[5]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[5]++;
				break;

			case  kCanMoveDirection::leftBottom:
				cannotMove_[kCanMoveDirection::leftBottom].localAdd[index[6]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[6]++;
				break;

			case  kCanMoveDirection::rightBottom:
				cannotMove_[kCanMoveDirection::rightBottom].localAdd[index[7]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[7]++;
				break;
			}
		}
	}

	for (int dire = 0; dire < 8; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			cannotMove_[dire].worldAdd[i] = cannotMove_[dire].localAdd[i] + worldAdd_;
		}
	}

}

void BullCow::MoveAreasUpdate() {
	for (int dire = 0; dire < 8; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			moveAreas_[dire].worldPos[i] = {
				cannotMove_[dire].worldAdd[i].x * tileSize_.x + (tileSize_.x / 2),
				cannotMove_[dire].worldAdd[i].y * tileSize_.y + (tileSize_.y / 2)
			};
		}
	}
}

void BullCow::DireInit() {
	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}
}

void BullCow::MoveTurn() {
	if (isTurnChange_) {
		isIdle_ = true;
	}
}

void BullCow::CheckMoveDire() {

	if (isIdle_) {

		maxDireValue_ = -99999;
		maxDireValueIndex_ = 0;
		adjoinNum_ = 0;

		//全方位囲まれていたら処理を終わる
		for (int i = 0; i < 8; i++) {
			if (canMoveDireValue_[i] < -900) {
				adjoinNum_++;
			}
		}

		if (adjoinNum_ == 8) {
			return;
		}

		// 一番値の大きい方向に動く
		for (int i = 0; i < 8; i++) {
			if (maxDireValue_ < canMoveDireValue_[i]) {
				maxDireValue_ = canMoveDireValue_[i];
				maxDireValueIndex_ = i;

				switch (maxDireValueIndex_) {
				case kCanMoveDirection::leftTop:
					moveGridNum_--;
					break;

				case kCanMoveDirection::rightTop:
					moveGridNum_--;
					break;

				case kCanMoveDirection::leftBottom:
					moveGridNum_--;
					break;

				case kCanMoveDirection::rightBottom:
					moveGridNum_--;
					break;
				}

			}
		}

		isIdle_ = false;

		isMove_ = true;

	}
}

void BullCow::Move() {
	if (isMove_) {
		if (easeT_ == 0.0f) {
			// 移動方向の量によって進む箇所を決める
			switch (maxDireValueIndex_) {
			case kCanMoveDirection::top:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x;
				endPos_.y = worldPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::top;
				break;

			case kCanMoveDirection::bottom:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x;
				endPos_.y = worldPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::bottom;
				break;

			case kCanMoveDirection::left:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x - tileSize_.y;
				endPos_.y = worldPos_.y;

				movedDire_ = kCanMoveDirection::left;
				break;

			case kCanMoveDirection::right:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x + tileSize_.y;
				endPos_.y = worldPos_.y;

				movedDire_ = kCanMoveDirection::right;
				break;

			case kCanMoveDirection::leftTop:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x - tileSize_.x;
				endPos_.y = worldPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::leftTop;
				break;

			case kCanMoveDirection::rightTop:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x + tileSize_.x;
				endPos_.y = worldPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::rightTop;
				break;

			case kCanMoveDirection::leftBottom:

				startPos_ = worldPos_;
				endPos_.x = worldPos_.x - tileSize_.x;
				endPos_.y = worldPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::leftBottom;
				break;

			case kCanMoveDirection::rightBottom:
				startPos_ = worldPos_;
				endPos_.x = worldPos_.x + tileSize_.x;
				endPos_.y = worldPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::leftBottom;
				break;
			}
		}

		// ここから下で移動する
		frameCount_++;

		easeT_ = frameCount_ / frameCountLimit_;

		worldPos_.x = MyMath::Lerp(easeT_, startPos_.x, endPos_.x);
		worldPos_.y = MyMath::Lerp(easeT_, startPos_.y, endPos_.y);

		if (frameCount_ >= frameCountLimit_) {
			easeT_ = 0;
			frameCount_ = 0;
			moveGridNum_--;
		}

		if (isFenceAttack_) {
			isFenceAttack_ = false;
		}

		// 2マス動いたら
		if (moveGridNum_ == 0) {
			// 移動の終了とともにturnがプレイヤー側になる
			turnType_ = kTurnType::Players;
			isMove_ = false;
			moveGridNum_ = 2;
		}

	} else {
		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;
		isMove_ = false;
		moveGridNum_ = 2;
		easeT_ = 0;
		frameCount_ = 0;
	}
}


void BullCow::ImguiDraw() {
	//リリースの時は消す
	ImGui::Begin("evaluateValue");

	if (ImGui::TreeNode("Value")) {
		ImGui::SliderInt("value_.wall:", &value_.wall, 0, 30);
		ImGui::SliderInt("value_.fourArea:", &value_.fourArea, 0, 30);
		ImGui::SliderInt("value_.allDire:", &value_.allDire, 0, 30);
		ImGui::SliderInt("value_.adjoin:", &value_.adjoin, -50, 50);
		ImGui::SliderInt("value_.adjoinAdd:", &value_.adjoinAdd, -50, 50);
		ImGui::SliderInt("value_.clamp:", &value_.clamp, 0, 5);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("hardle")) {
		ImGui::SliderInt("value_.dog:", &value_.dog, 100, 1500);
		ImGui::SliderInt("value_.rock:", &value_.rock, 100, 200);
		ImGui::SliderInt("value_.fence:", &value_.fence, -100, 100);
		ImGui::TreePop();
	}



	ImGui::End();

	//
	ImGui::Begin("canMoveDireValue");

	for (int dire = 0; dire < 8; dire++) {
		ImGui::Text("%d : %s", canMoveDireValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::End();

	//
	ImGui::Begin("gridDistanceValue_");
	ImGui::BeginChild(ImGuiWindowFlags_NoTitleBar);

	for (int dire = 0; dire < 8; dire++) {
		ImGui::Text("%d : %s", gridDistanceValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::EndChild();
	ImGui::End();

}
