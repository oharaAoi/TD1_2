#include "Fighting.h"

BullFighting::BullFighting() { Init(); }

BullFighting::~BullFighting() { Finalize(); }

void BullFighting::Finalize() {
}

/*========================================================
	初期化関数
========================================================*/
void BullFighting::Init() {

	// ワールド空間での中心座標
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::BULLFIGHTING) {
				worldPos_ = {
					col * tileSize_.x + (tileSize_.x * 0.5f),
					row * tileSize_.y + (tileSize_.y * 0.5f)
				};
			}
		}
	}

	moveGrid_ = LoadFile("./Resources/cow/fightingDireRange.csv");

	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {
			switch (moveGrid_[row][col]) {
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

			case 50:
				localCenterAdd_.x = col;
				localCenterAdd_.y = row;
				break;
			}
		}
	}

	// 各方向を調べる数を配列に入れる
	for (int i = 0; i < 4; i++) {
		cannotMove_[i].localAdd.resize(direAddressNum_[i]);
		cannotMove_[i].worldAdd.resize(direAddressNum_[i]);
	}

	CenterAddUpdate();

	// 移動状態
	isIdle_ = false;
	isMove_ = false;
	isStan_ = true;

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ -tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
	};

	// ワールド行列の生成
	MakeWorldMatrix();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		nearWallOfValue_[i] = 0;
	}

	gh_ = Novice::LoadTexture("white1x1.png");

	// 移動方向/量

	moveScalar_.x = 3;
	moveScalar_.y = 3;

	//=======================================================================================
	// csvで方向の評価
	moveGrid_ = LoadFile("./Resources/cow/fightingDireRange.csv");

	//=======================================================================================
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
	value_.fourArea = 5;
	value_.allDire = 15;
	value_.clamp = 2;

	value_.dog = 150;
	value_.rock = 1000;
	value_.adjoin = 150;
	value_.adjoinAdd = 200;
	value_.fence = 10;
	value_.slantFence = -200;

	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	isFenceAttack_ = false;
	//=======================================================================================
	//　移動で使う
	startPos_ = { 0.0f, 0.0f };
	endPos_ = { 0.0f, 0.0f };

	easeT_ = 0.0f;
	frameCount_ = 0.0f;

	moveGridNum_ = 3;
}

/*========================================================
	更新処理関数
========================================================*/
void BullFighting::Update() {
	// 各頂点のアドレスの計算
	CenterAddUpdate();

	// 牛がスタン状態だったら早期リターン
	if (CheckIsStan()) {return;}

	// 牛のターンか
	MoveTurn();

	// 移動
	CheckMoveDire();

	Move();

	//　移動後の各頂点のアドレスの計算
	CenterAddUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

	/*DireInit();*/
}

/*========================================================
	描画処理関数
========================================================*/
void BullFighting::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFF00FF);

	ImguiDraw();

}

/*========================================================
	座標系関数
========================================================*/
void BullFighting::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void BullFighting::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);
}

/*========================================================
	処理関数
========================================================*/

void BullFighting::DireInit() {
	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	// 進行方向に人がいるか
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			isMoveDireBlock_[i][j] = false;
			moveDireOnPreson_[i][j] = false;
		}
	}
}

bool BullFighting::CheckIsStan() {
	if (isTurnChange_) {
		if (isStan_) {
			// 移動の終了とともにturnがプレイヤー側になる
			turnType_ = kTurnType::Players;

			isStan_ = false;

			return true;
		}
	}
	return false;
}

void BullFighting::MoveTurn() {
	if (!isIdle_) {
		if (isTurnChange_) {
			isIdle_ = true;
		}
	}
}

void BullFighting::CheckMoveDire() {

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
		for (int i = 0; i < 4; i++) {
			if (maxDireValue_ < canMoveDireValue_[i]) {
				maxDireValue_ = canMoveDireValue_[i];
				maxDireValueIndex_ = i;
			}
		}

		// 待機状態の解除
		isIdle_ = false;

		isMove_ = true;
	}
}

void BullFighting::Move() {
	if (isMove_) {

		if (easeT_ == 0.0f) {
			// 移動方向の量によって進む箇所を決める
			switch (maxDireValueIndex_) {
			case kCanMoveDirection::top:
				movedDire_ = kCanMoveDirection::top;

				startPos_ = worldPos_;
				endPos_.x = worldPos_.x;
				endPos_.y = worldPos_.y + tileSize_.y;

				break;

			case kCanMoveDirection::bottom:
				movedDire_ = kCanMoveDirection::bottom;

				startPos_ = worldPos_;
				endPos_.x = worldPos_.x;
				endPos_.y = worldPos_.y - tileSize_.y;
				break;

			case kCanMoveDirection::left:
				movedDire_ = kCanMoveDirection::left;

				startPos_ = worldPos_;
				endPos_.x = worldPos_.x - tileSize_.y;
				endPos_.y = worldPos_.y;
				break;

			case kCanMoveDirection::right:
				movedDire_ = kCanMoveDirection::right;

				startPos_ = worldPos_;
				endPos_.x = worldPos_.x + tileSize_.y;
				endPos_.y = worldPos_.y;
				break;
			}
		}

		// ここから下で移動する
		frameCount_++;

		easeT_ = frameCount_ / 30.0f;

		worldPos_.x = MyMath::Lerp(easeT_, startPos_.x, endPos_.x);
		worldPos_.y = MyMath::Lerp(easeT_, startPos_.y, endPos_.y);

		if (frameCount_ >= 30) {
			easeT_ = 0;
			frameCount_ = 0;
			moveGridNum_--;
		}

		// フェンスの処理
		if (isFenceAttack_) {
			isFenceAttack_ = false;
		}

		// 三マス動いたら
		if (moveGridNum_ == 0) {
			// 移動の終了とともにturnがプレイヤー側になる
			turnType_ = kTurnType::Players;
			isMove_ = false;
			moveGridNum_ = 3;
		}
	} else {
		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;
		isMove_ = false;
		moveGridNum_ = 3;
		easeT_ = 0;
		frameCount_ = 0;
	}
}

void BullFighting::CenterAddUpdate() {
	worldAdd_ = {
		static_cast<int>(worldPos_.x / tileSize_.x),
		static_cast<int>(worldPos_.y / tileSize_.y)
	};

	int index[4] = { 0 };

	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {
			switch (moveGrid_[row][col]) {
			case kCanMoveDirection::top:
				cannotMove_[kCanMoveDirection::top].localAdd[index[0]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[0]++;
				break;

			case  kCanMoveDirection::bottom:
				cannotMove_[kCanMoveDirection::bottom].localAdd[index[1]] = { col - localCenterAdd_.x,(row - localCenterAdd_.y) };
				index[1]++;
				break;

			case  kCanMoveDirection::left:
				cannotMove_[kCanMoveDirection::left].localAdd[index[2]] = { (col - localCenterAdd_.x),row - localCenterAdd_.y };
				index[2]++;
				break;

			case  kCanMoveDirection::right:
				cannotMove_[kCanMoveDirection::right].localAdd[index[3]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[3]++;
				break;
			}
		}
	}
	std::reverse(cannotMove_[kCanMoveDirection::bottom].localAdd.begin(), cannotMove_[kCanMoveDirection::bottom].localAdd.end());
	std::reverse(cannotMove_[kCanMoveDirection::left].localAdd.begin(), cannotMove_[kCanMoveDirection::left].localAdd.end());

	for (int dire = 0; dire < 4; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			cannotMove_[dire].worldAdd[i] = cannotMove_[dire].localAdd[i] + worldAdd_;
		}
	}
}

void BullFighting::ImguiDraw() {
	//リリースの時は消す
	ImGui::Begin("evaluateValue");

	ImGui::SliderInt("value_.wall:", &value_.wall, 0, 30);
	ImGui::SliderInt("value_.fourArea:", &value_.fourArea, 0, 30);
	ImGui::SliderInt("value_.allDire:", &value_.allDire, 0, 30);

	ImGui::SliderInt("value_.dog:", &value_.dog, 100, 1500);
	ImGui::SliderInt("value_.rock:", &value_.rock, 100, 200);
	ImGui::SliderInt("value_.fence:", &value_.fence, -100, 100);
	ImGui::SliderInt("value_.adjoin:", &value_.adjoin, -100, 100);

	ImGui::End();

	//
	ImGui::Begin("canMoveDireValue");

	for (int dire = 0; dire < 4; dire++) {
		ImGui::Text("%d : %s", canMoveDireValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::End();

	
	/*ImGui::Begin("gridDistanceValue_");
	ImGui::BeginChild(ImGuiWindowFlags_NoTitleBar);

	for (int dire = 0; dire < 8; dire++) {
		ImGui::Text("%d : %s", gridDistanceValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::EndChild();
	ImGui::End();*/

}


