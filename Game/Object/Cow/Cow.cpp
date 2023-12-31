﻿#include "Cow.h"

Cow::Cow() { Init(); }

Cow::~Cow() { Finalize(); }


/*==========================================================
	初期化関数
==========================================================*/
void Cow::Init() {

	// ワールド空間での中心座標
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::COW) {
				worldCenterPos_ = {
					col * tileSize_.x + (tileSize_.x * 0.5f),
					row * tileSize_.y + (tileSize_.y * 0.5f)
				};
			}
		}
	}

	worldPreCenterPos_ = worldCenterPos_;

	gh_ = Novice::LoadTexture("white1x1.png");

	isIdle_ = false;
	isMove_ = false;

	// csvを読み込んで評価するための変数の初期化
	evaluateGrid_ = LoadFile("./Resources/cow/cowEightDireRange.csv");

	// ローカルアドレスでの牛の位置を保存
	for (int row = 0; row < evaluateGrid_.size(); row++) {
		for (int col = 0; col < evaluateGrid_[0].size(); col++) {
			switch (evaluateGrid_[row][col]) {
			case 50:
				localCenterAdd_.x = col;
				localCenterAdd_.y = row;
				break;
			}
		}
	}

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ -tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
	};

	MakeWorldMatrix();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// アドレスの初期化
	CenterAddUpdate();

	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	moveScalar_.x = 1;
	moveScalar_.y = 1;

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

	value_.dog = 1500;
	value_.rock = 10000;
	value_.fence = 100;
	value_.slantFence = -200;
	value_.adjoin = 600;
	value_.adjoinAdd = 200;


	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	//=========================================
	// フェンスとの判定で使う
	isFenceAttack_ = false;

	//=========================================
	//　移動で使う
	startPos_ = { 0.0f, 0.0f };
	endPos_ = { 0.0f, 0.0f };

	easeT_ = 0.0f;
	frameCount_ = 0.0f;

	moveGridNum_ = 1;
}


/*==========================================================
	更新処理関数
==========================================================*/
void Cow::Update() {

	// ターンが牛側でなければ早期returnする
	/*if (turnType_ != kTurnType::Cows) { return; }*/

	// 牛の現在の位置を取得
	CenterAddUpdate();

	// デバック用
	CowMove();

	// 動く場所を計算
	CheckMoveDire();

	// 実際に動く
	Move();

	// 移動後のアドレスを計算する
	CenterAddUpdate();

	// 動ける範囲の座標の計算
	MoveAreasUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();
}


/*==========================================================
	描画処理関数
==========================================================*/
void Cow::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFFFF);

	for (int i = 0; i < 8; i++) {
		Draw::Quad(moveAreas_[i].screenVertex, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFF44);
	}

	DebugScreenPrintf();

	ImguiDraw();
}

/*==========================================================
	終了処理関数
==========================================================*/
void Cow::Finalize() {


}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cow::CowMove() {
	if (Inputs::IsTriggerKey(DIK_M) or isTurnChange_) {
		isIdle_ = true;
	}
}

void Cow::CheckMoveDire() {
	if (isIdle_) {

		CheckNearWall();

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

void Cow::Move() {
	if (isMove_) {

		if (easeT_ == 0.0f) {

			// 移動方向の量によって進む箇所を決める
			switch (maxDireValueIndex_) {
			case kCanMoveDirection::top:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x;
				endPos_.y = worldCenterPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::top;
				break;

			case kCanMoveDirection::bottom:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x;
				endPos_.y = worldCenterPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::bottom;
				break;

			case kCanMoveDirection::left:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x - tileSize_.x;
				endPos_.y = worldCenterPos_.y;

				movedDire_ = kCanMoveDirection::left;
				break;

			case kCanMoveDirection::right:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x + tileSize_.x;
				endPos_.y = worldCenterPos_.y;

				movedDire_ = kCanMoveDirection::right;
				break;

			case kCanMoveDirection::leftTop:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x - tileSize_.x;
				endPos_.y = worldCenterPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::leftTop;
				break;

			case kCanMoveDirection::rightTop:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x + tileSize_.x;
				endPos_.y = worldCenterPos_.y + tileSize_.y;

				movedDire_ = kCanMoveDirection::rightTop;
				break;

			case kCanMoveDirection::leftBottom:

				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x - tileSize_.x;
				endPos_.y = worldCenterPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::leftBottom;
				break;

			case kCanMoveDirection::rightBottom:
				startPos_ = worldCenterPos_;
				endPos_.x = worldCenterPos_.x + tileSize_.x;
				endPos_.y = worldCenterPos_.y - tileSize_.y;

				movedDire_ = kCanMoveDirection::leftBottom;
				break;
			}
		}

		// ここから下で移動する
		frameCount_++;

		easeT_ = frameCount_ / 15.0f;

		worldCenterPos_.x = MyMath::Lerp(easeT_, startPos_.x, endPos_.x);
		worldCenterPos_.y = MyMath::Lerp(easeT_, startPos_.y, endPos_.y);

		if (frameCount_ >= 15) {
			easeT_ = 0;
			frameCount_ = 0;
			moveGridNum_--;
		}

		if (isFenceAttack_) {
			isFenceAttack_ = false;
		}

		if (moveGridNum_ == 0) {
			// 移動の終了とともにturnがプレイヤー側になる
			turnType_ = kTurnType::Players;
			isMove_ = false;
			moveGridNum_ = 1;
		}
	} else {
		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;
		isMove_ = false;
		moveGridNum_ = 1;
		easeT_ = 0;
		frameCount_ = 0;
	}
}

// ------ 今のアドレスの計算 ------ //
void Cow::CenterAddUpdate() {
	centerAdd_ = {
		static_cast<int>(worldCenterPos_.x / tileSize_.x),
		static_cast<int>(worldCenterPos_.y / tileSize_.y)
	};

	// 牛のアドレスから8方向のアドレスをcantMova_[dire]に保存する
	for (int row = 0; row < evaluateGrid_.size(); row++) {
		for (int col = 0; col < evaluateGrid_[0].size(); col++) {
			switch (evaluateGrid_[row][col]) {
			case kCanMoveDirection::top:
				cannotMove_[kCanMoveDirection::top].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::bottom:
				cannotMove_[kCanMoveDirection::bottom].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::left:
				cannotMove_[kCanMoveDirection::left].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::right:
				cannotMove_[kCanMoveDirection::right].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::leftTop:
				cannotMove_[kCanMoveDirection::leftTop].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::rightTop:
				cannotMove_[kCanMoveDirection::rightTop].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::leftBottom:
				cannotMove_[kCanMoveDirection::leftBottom].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;

			case  kCanMoveDirection::rightBottom:
				cannotMove_[kCanMoveDirection::rightBottom].localAdd = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				break;
			}
		}
	}

	for (int dire = 0; dire < 8; dire++) {
		cannotMove_[dire].worldAdd_ = cannotMove_[dire].localAdd + centerAdd_;
	}

}

/* ------ 動ける範囲の座標を計算する ------ */
void Cow::MoveAreasUpdate() {
	for (int dire = 0; dire < 8; dire++) {
		moveAreas_[dire].worldPos = {
			cannotMove_[dire].worldAdd_.x * tileSize_.x + (tileSize_.x / 2),
			cannotMove_[dire].worldAdd_.y * tileSize_.y + (tileSize_.y / 2)
		};
	}
}


// ------ 方向の初期化 ------ //
void Cow::DireInit() {
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

void Cow::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

	for (int i = 0; i < 8; i++) {
		moveAreas_[i].screenMatrix = MakeWvpVpMatrix(moveAreas_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		moveAreas_[i].screenVertex.lt = Transform(localVertex_.lt, moveAreas_[i].screenMatrix);
		moveAreas_[i].screenVertex.rt = Transform(localVertex_.rt, moveAreas_[i].screenMatrix);
		moveAreas_[i].screenVertex.lb = Transform(localVertex_.lb, moveAreas_[i].screenMatrix);
		moveAreas_[i].screenVertex.rb = Transform(localVertex_.rb, moveAreas_[i].screenMatrix);

	}
}

void Cow::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

	for (int i = 0; i < 8; i++) {
		moveAreas_[i].worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, moveAreas_[i].worldPos);
	}

}

//	マス目の計算で求めたものをクランプで10の間に収める
void Cow::CheckNearPerson() {
	for (int i = 0; i < 8; i++) {
		if (gridDistanceValue_[i] != 0) {
			canMoveDireValue_[i] += (10 - std::clamp(gridDistanceValue_[i], 0, 10)) * value_.clamp;
		}
	}
}

// 一番近い壁までの計算
void Cow::CheckNearWall() {

	// 近い壁を計算する
	nearWallOfValue_[kCanMoveDirection::top] = row_ - centerAdd_.y - 1;
	nearWallOfValue_[kCanMoveDirection::bottom] = centerAdd_.y;

	nearWallOfValue_[kCanMoveDirection::left] = centerAdd_.x;
	nearWallOfValue_[kCanMoveDirection::right] = col_ - centerAdd_.x - 1;


	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;

	// 同値があった時のフラグ
	wallIsDuplicate_ = false;

	// 一番低い値を求める
	for (int i = 0; i < 4; i++) {
		if (wallMin_ > nearWallOfValue_[i]) {
			wallMin_ = nearWallOfValue_[i];
			wallMinIndex_ = i;

		} else if (wallMin_ == nearWallOfValue_[i]) {
			wallMinNum_++;
			wallIsDuplicate_ = true;
		}
	}

	// 同値がなかった時はそれぞれの辺だけで済む
	if (!wallIsDuplicate_) {
		switch (wallMinIndex_) {
		case kCanMoveDirection::top:
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

			break;

		case kCanMoveDirection::bottom:
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
			break;

		case kCanMoveDirection::left:
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			break;

		case kCanMoveDirection::right:
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
			break;
		}
	}

	/// 2つ同値があった時
	if (wallMinNum_ == 2) {
		if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::right] &&
			nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::left]) {

			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::left] &&
			nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::right]) {

			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::left] == nearWallOfValue_[kCanMoveDirection::top] &&
			nearWallOfValue_[kCanMoveDirection::left] == nearWallOfValue_[kCanMoveDirection::bottom]) {

			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::right] == nearWallOfValue_[kCanMoveDirection::top] &&
			nearWallOfValue_[kCanMoveDirection::right] == nearWallOfValue_[kCanMoveDirection::bottom]) {

			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;

		}

	} else if (wallMinNum_ == 1) {
		if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::left]) {
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::right]) {
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::left]) {
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::right]) {
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
		}
	}
}

void Cow::DebugScreenPrintf() {

	Novice::ScreenPrintf(1000, 450, "Keys[DIK_M]:cowMove");

	Novice::ScreenPrintf(1000, 500, "isFenceAttack:%d", isFenceAttack_);

	for (int i = 0; i < 4; i++) {
		Novice::ScreenPrintf(1100, 30 + (i * 20), "nearWall:%d", nearWallOfValue_[i]);
	}


}

void Cow::ImguiDraw() {
	//リリースの時は消す
	ImGui::Begin("evaluateValue");

	if (ImGui::TreeNode("Value")) {
		ImGui::SliderInt("value_.wall:", &value_.wall, 0, 30);
		ImGui::SliderInt("value_.fourArea:", &value_.fourArea, 0, 30);
		ImGui::SliderInt("value_.allDire:", &value_.allDire, 0, 30);
		ImGui::SliderInt("value_.dog:", &value_.dog, 100, 1500);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("rock")) {
		ImGui::SliderInt("value_.rock:", &value_.rock, 500, 2000);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Fence")) {
		ImGui::SliderInt("value_.fence:", &value_.fence, -200, 200);
		ImGui::SliderInt("value_.NANAMEslantFence:", &value_.slantFence, -300, 100);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Tonari")) {
		ImGui::SliderInt("value_.adjoin:", &value_.adjoin, 100, 800);
		ImGui::SliderInt("value_.adjoinAdd:", &value_.adjoinAdd, 100, 400);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sonota")) {
		ImGui::SliderInt("value_.clamp:", &value_.clamp, 0, 5);
		ImGui::SliderInt("moveScalar_.x:", &moveScalar_.x, 0, 5);
		ImGui::SliderInt("moveScalar_.y:", &moveScalar_.y, 0, 5);
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
