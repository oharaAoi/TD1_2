#include "Riata.h"

Riata::Riata() { Init(); }

Riata::~Riata() { Finalize(); }


/*================================================
	初期化関数
================================================*/
void Riata::Init() {

	worldPos_ = { 0.0f,0.0f };
	size_ = { 64.0f,64.0f };
	gh_ = Novice::LoadTexture("white1x1.png");
	color_ = 0x00dd0080;

	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f }
	};

	MakeWorldMatrix();

	screenMatrix_ = worldMatrix_;
	screenVertex_ = worldVertex_;

	isIdle_ = false;
	isStart_ = false;

	moveDir_ = { 0.0f,0.0f };

	startingPos_ = { 0.0f,0.0f };
	destinationPos_ = { 0.0f,0.0f };

}


/*================================================
	更新処理関数
================================================*/
void Riata::Update() {

	if (!isStart_) { return; }

	worldPos_ += moveDir_ * Vec2f{ 4,4 };

	MakeWorldMatrix();

}


/*================================================
	描画処理関数
================================================*/
void Riata::Draw() {

	// スクリーン座標系での描画
	Draw::Quad(
		screenVertex_,
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		gh_,
		color_
	);

}


/*================================================
	終了処理関数
================================================*/
void Riata::Finalize() {


}



/*================================================
	その他メンバ関数
================================================*/


void Riata::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);
	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);
}

void Riata::MakeScreenMatrix(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {
	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);
	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);
}
