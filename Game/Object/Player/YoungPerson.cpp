#include "YoungPerson.h"

YoungPerson::YoungPerson() { Init(); }

YoungPerson::~YoungPerson() { Finalize(); }

/*================================================================
	初期化関数
================================================================*/
void YoungPerson::Init() {


	// ワールド空間での中心点
	size_ = tileSize_;
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::YANGMAN) {
				worldCenterPos_ = {
					col * tileSize_.x + (size_.x * 0.5f),
					row * tileSize_.y + (size_.y * 0.5f)
				};
			}
		}
	}
	gh_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");

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

	// 移動できる状態かどうか
	isMoveIdle_ = false;

	// 移動方向
	moveDir_.x = 0.0f;
	moveDir_.y = 0.0f;

	// 移動量
	moveValue_.x = 1.0f;
	moveValue_.y = 1.0f;

	for (int i = 0; i < 4; i++) {
		canMoveDir_[i] = false;
	}

}


/*================================================================
	更新処理関数
================================================================*/
void YoungPerson::Update() {

	Move();

}


/*================================================================
	描画処理関数
================================================================*/
void YoungPerson::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,64.0f }, { 64.0f,64.0f }, gh_, 0xFFFFFFFF);

}


/*================================================================
	終了処理関数
================================================================*/
void YoungPerson::Finalize() {



}


/*================================================================
	その他メンバ関数
================================================================*/

void YoungPerson::Move(){

	if (isMoveIdle_) {

		// 移動待機状態の解除
		if (input->IsTriggerMouse(1)) {
			isMoveIdle_ = false;
		}

	} else {

		if (input->IsTriggerMouse(0)) {
			// 移動待機状態にする
			isMoveIdle_ = true;

			// 方向を取得

		}

	}

}

void YoungPerson::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void YoungPerson::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}
