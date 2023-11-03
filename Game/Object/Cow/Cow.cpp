﻿#include "Cow.h"

Cow::Cow(MapChip* mapChip) { Init(mapChip); }

Cow::~Cow() { Finalize(); }


/*==========================================================
	初期化関数
==========================================================*/
void Cow::Init(MapChip* mapChip) {

	// ワールド空間での中心座標
	size_ = mapChip->GetTileSize();
	for (int row = 0; row < mapChip->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip->GetMapChipCol(); col++) {

			if (mapChip->GetMapChipAdd()[row][col] == ChipType::COW) {
				worldCenterPos_ = {
					col * mapChip->GetTileSize().x + (size_.x * 0.5f),
					row * mapChip->GetTileSize().y + (size_.y * 0.5f)
				};
			}
		}
	}

	gh_ = Novice::LoadTexture("white1x1.png");

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f },
	};

	MakeWorldMatrix();

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

	for (int i = 0; i < 4; i++) {
		nearWallOfValue_[i] = 0;
	}

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// アドレスの初期化
	CenterAddUpdate();


	//=========================================
	// 評価値(ここの数値をいじればいける)
	value_.wall = 3;


	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;
}


/*==========================================================
	更新処理関数
==========================================================*/
void Cow::Update() {

	// 牛の現在の位置を取得
	CenterAddUpdate();

	// 動く方向の評価をする

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

	// 牛の方向をリセットする
	DireInit();
}


/*==========================================================
	描画処理関数
==========================================================*/
void Cow::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFFFF);

}


/*==========================================================
	終了処理関数
==========================================================*/
void Cow::Finalize() {


}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cow::CenterAddUpdate() {
	centerAdd_ = {
		static_cast<int>(worldCenterPos_.x / size_.x),
		static_cast<int>(worldCenterPos_.y / size_.y)
	};
}

void Cow::DireInit() {
	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
	}
}

void Cow::CheckNearPerson() {
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] += (10 - std::clamp(gridDistanceValue_[i], 0, 10)) * 2;
	}
}

void Cow::CheckNearWall() {

	// 近い壁を計算する
	nearWallOfValue_[kCanMoveDirection::top] = (row_ - centerAdd_.y);
	nearWallOfValue_[kCanMoveDirection::bottom] = centerAdd_.y - 1;
	nearWallOfValue_[kCanMoveDirection::left] = centerAdd_.x - 1;
	nearWallOfValue_[kCanMoveDirection::right] = (col_ - centerAdd_.x);


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
			
		}else if(wallMin_ == nearWallOfValue_[i]){
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

void Cow::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void Cow::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}
