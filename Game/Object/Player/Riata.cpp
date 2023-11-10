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

}


/*================================================
	更新処理関数
================================================*/
void Riata::Update() {

	if (input->IsPressKey(DIK_UP)) { worldPos_.y += 4.0f; }
	if (input->IsPressKey(DIK_DOWN)) { worldPos_.y -= 4.0f; }
	if (input->IsPressKey(DIK_LEFT)) { worldPos_.x -= 4.0f; }
	if (input->IsPressKey(DIK_RIGHT)) { worldPos_.x += 4.0f; }

	MakeWorldMatrix();

	if (isIdle_) {

		if (isStart_) {

			// 終了条件
			// if(){isStart = false; isIdle = false;}

		} else {

			// 移動始める条件
			/*if (input->IsTriggerKey(DIK_2)) {
				isStart_ = true;
			}*/

		}

	} else {

		// 移動待機状態になる条件
		/*if (input->IsTriggerKey(DIK_1)) {
			isIdle_ = true;
		}*/

	}
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
