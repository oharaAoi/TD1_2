﻿#include "Cowherd.h"

Cowherd::Cowherd(MapChip* mapChip) { Init(mapChip); }

Cowherd::~Cowherd() { Finalize(); }

/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Cowherd::Init(MapChip* mapChip) {

	// ワールド空間での中心点
	size_ = mapChip->GetTileSize();
	for (int row = 0; row < mapChip->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip->GetMapChipCol(); col++) {

			if (mapChip->GetMapChipAdd()[row][col] == ChipType::COWHERD) {
				worldCenterPos_ = {
					col * mapChip->GetTileSize().x + (size_.x * 0.5f),
					row * mapChip->GetTileSize().y + (size_.y * 0.5f)
				};
			}
		}
	}
	gh_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");

	// 各空間の頂点
	localVertex_.lt = { -size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.rt = { size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.lb = { -size_.x * 0.5f, -size_.y * 0.5f };
	localVertex_.rb = { size_.x * 0.5f, -size_.y * 0.5f };

	// ローカル以外の各空間の行列
	MakeWorldMatrix();

	screenMatrix_ = worldMatrix_;
	screenVertex_ = worldVertex_;

	isMoveIdle_ = false;

}


/*==========================================================
	更新処理
==========================================================*/
void Cowherd::Update() {


	MakeWorldMatrix();

}


/*==========================================================
	描画処理
==========================================================*/
void Cowherd::Draw() {

	Draw::Quad(screenVertex_, { 128,0.0f }, { 64.0f,64.0f }, gh_, 0xFFFFFFFF);

}


/*==========================================================
	終了処理
==========================================================*/
void Cowherd::Finalize() {



}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cowherd::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);


}

void Cowherd::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}


void Cowherd::Move() {

	if (isMoveIdle_) {

		// いつでも移動待機状態をクリアできるよにする
		if (input->IsTriggerMouse(1)) {
			isMoveIdle_ = false;
		}

		// 上下左右のマスとマウスの当たり判定をとりどこをクリックしたかで移動先を決める




	} else {

		// スクリーン座標上でマウスの位置がオブジェクトの上にあれば当たっている判定になる
		if (Collision::Rect::Point(screenVertex_,
			{ static_cast<float>(input->GetMousePos().x), static_cast<float>(input->GetMousePos().y) })) {

			// クリックしたとき
			if (input->IsTriggerMouse(0)) {

				// 移動待機状態になる
				isMoveIdle_ = true;

			}

		}

	}

}
