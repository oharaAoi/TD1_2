﻿#include "Dog.h"

Dog::Dog() { Init(); }

Dog::~Dog() { Finalize(); }


/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Dog::Init() {

	// ワールド空間での中心座標
	worldCenterPos_ = { 640.0f,360.0 };
	size_ = { 32.0f,32.0f };
	gh_ = Novice::LoadTexture("white1x1.png");

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


}


/*==========================================================
	更新処理関数
==========================================================*/
void Dog::Update() {


	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

}


/*==========================================================
	描画処理関数
==========================================================*/
void Dog::Draw() {

	// 犬の描画
	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFFFF);

}


/*==========================================================
	終了処理関数
==========================================================*/
void Dog::Finalize() {


}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Dog::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}

void Dog::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}

