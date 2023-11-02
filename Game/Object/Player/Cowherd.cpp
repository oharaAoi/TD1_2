#include "Cowherd.h"

Cowherd::Cowherd() { Init(); }

Cowherd::~Cowherd() { Finalize(); }

/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Cowherd::Init() {

	// ワールド空間での中心点
	worldCenterPos_ = { 640.0f,360.0f };
	size_ = { 32.0f,32.0f };


	// 各空間の頂点
	localVertex_.lt = { -size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.rt = { size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.lb = { -size_.x * 0.5f, -size_.y * 0.5f };
	localVertex_.rb = { size_.x * 0.5f, -size_.y * 0.5f };

	// ローカル以外の各空間の行列
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f },0.0f,worldCenterPos_);
	screenMatrix_ = worldMatrix_;

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

	screenVertex_ = worldVertex_;


}


/*==========================================================
	更新処理
==========================================================*/
void Cowherd::Update() {



}


/*==========================================================
	描画処理
==========================================================*/
void Cowherd::Draw() {



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
