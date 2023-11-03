#include "Cowherd.h"

Cowherd::Cowherd() { Init(); }

Cowherd::~Cowherd() { Finalize(); }

/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Cowherd::Init() {

	// ワールド空間での中心点
	size_ = tileSize_;
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::COWHERD) {
				worldCenterPos_ = {
					col * tileSize_.x + (size_.x * 0.5f),
					row * tileSize_.y + (size_.y * 0.5f)
				};
			}
		}
	}
	centerAdd_ = worldCenterPos_ / size_;
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

	Move();

	MakeWorldMatrix();

}


/*==========================================================
	描画処理
==========================================================*/
void Cowherd::Draw() {

	Draw::Quad(screenVertex_, { 128,0.0f }, { 64.0f,64.0f }, gh_, 0xFFFFFFFF);

	DebugDraw();
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

				// 移動できる方向を所得


			}

		}

	}

}

void Cowherd::DebugDraw() {

	Novice::ScreenPrintf(100, 100, "isMoveIdle = %d", isMoveIdle_);

	Novice::ScreenPrintf(100, 120, "centerAdd : x = %d, y = %d", centerAdd_.x, centerAdd_.y);


}
