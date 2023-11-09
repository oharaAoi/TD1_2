#include "BullCow.h"

BullCow::BullCow() { Init(); }

BullCow::~BullCow() { Finalize(); }

void BullCow::Finalize(){

}

void BullCow::Init(){

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

	cannotMoveGrid_ = LoadFile("./Resources/cow/bullEightDireRange.csv");

	gh_ = Novice::LoadTexture("white1x1.png");

	isIdle_ = false;

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

	value_.dog = 150;
	value_.rock = 1000;
	value_.adjoin = 150;
	value_.fence = 200;
	value_.slantFence = -200;

	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	isFenceAttack_ = false;

}

void BullCow::Update(){

	// 牛の現在の位置を取得
	CenterAddUpdate();

	MoveTurn();

	// 動く
	Move();

	// 移動後のアドレスを計算する
	CenterAddUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

}

void BullCow::Draw(){
	if (isDisplay_) {
		Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFF0000FF);
	}

	ImguiDraw();
}

void BullCow::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void BullCow::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}

// ------ 今のアドレスの計算 ------ //
void BullCow::CenterAddUpdate() {
	worldAdd_ = {
		static_cast<int>(worldPos_.x / tileSize_.x),
		static_cast<int>(worldPos_.y / tileSize_.y)
	};
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

void BullCow::Move() {
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
			}
		}

		// 移動方向の量によって進む箇所を決める
		switch (maxDireValueIndex_) {
		case kCanMoveDirection::top:
			worldPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::top;
			break;

		case kCanMoveDirection::bottom:
			worldPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::bottom;
			break;

		case kCanMoveDirection::left:
			worldPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::left;
			break;

		case kCanMoveDirection::right:
			worldPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::right;
			break;

		case kCanMoveDirection::leftTop:
			worldPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			worldPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftTop;
			break;

		case kCanMoveDirection::rightTop:
			worldPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			worldPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::rightTop;
			break;

		case kCanMoveDirection::leftBottom:
			worldPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			worldPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftBottom;
			break;

		case kCanMoveDirection::rightBottom:
			worldPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			worldPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftBottom;
			break;
		}

		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;

		// 待機状態の解除
		isIdle_ = false;

		if (isFenceAttack_) {
			isFenceAttack_ = false;
		}
	}
}


void BullCow::ImguiDraw() {
	//リリースの時は消す
	ImGui::Begin("evaluateValue");

	ImGui::SliderInt("value_.wall:", &value_.wall, 0, 30);
	ImGui::SliderInt("value_.fourArea:", &value_.fourArea, 0, 30);
	ImGui::SliderInt("value_.allDire:", &value_.allDire, 0, 30);
	ImGui::SliderInt("value_.clamp:", &value_.clamp, 0, 5);


	ImGui::SliderInt("value_.dog:", &value_.dog, 100, 1500);
	ImGui::SliderInt("value_.rock:", &value_.rock, 100, 200);
	ImGui::SliderInt("value_.fence:", &value_.fence, -100, 100);
	ImGui::SliderInt("value_.adjoin:", &value_.adjoin, -100, 100);


	ImGui::SliderInt("moveScalar_.x:", &moveScalar_.x, 0, 5);
	ImGui::SliderInt("moveScalar_.y:", &moveScalar_.y, 0, 5);

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
