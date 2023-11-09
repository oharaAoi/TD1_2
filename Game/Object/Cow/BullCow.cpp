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
					col * tileSize_.x + (size_.x * 0.5f),
					row * tileSize_.y + (size_.y * 0.5f)
				};
			}
		}
	}

	gh_ = Novice::LoadTexture("white1x1.png");

	isIdle_ = false;

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f },
	};

	MakeWorldMatrix();

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

}

void BullCow::Update()
{
}

void BullCow::Draw(){

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFF0000FF);
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
		static_cast<int>(worldPos_.x / size_.x),
		static_cast<int>(worldPos_.y / size_.y)
	};
}
