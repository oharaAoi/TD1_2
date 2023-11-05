#include "Dog.h"

Dog::Dog() { Init(); }

Dog::~Dog() { Finalize(); }


/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Dog::Init() {

	// ワールド空間での中心座標
	worldCenterPos_ = { static_cast<float>(input->GetMousePos().x),
						static_cast<float>(input->GetMousePos().y) };
	size_ = { 64.0f,64.0f };
	gh_ = Novice::LoadTexture("white1x1.png");

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f },
	};

	isExist_ = false;


	isExistSide_ = NOT;

	MakeWorldMatrix();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	drawMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;


	//================================================================
	// 
	// top
	putPlace_[0].worldPos.x = (col_ / 2) * tileSize_.x + (tileSize_.x / 2);
	putPlace_[0].worldPos.y = row_ * tileSize_.y - (tileSize_.y / 2.0f);

	// bottom
	putPlace_[1].worldPos.x = (col_ / 2) * tileSize_.x + (tileSize_.x / 2);
	putPlace_[1].worldPos.y = tileSize_.y - (tileSize_.y / 2.0f);

	// left
	putPlace_[2].worldPos.x = (tileSize_.x / 2);
	putPlace_[2].worldPos.y = (row_ / 2) * tileSize_.y + (tileSize_.y / 2.0f);

	// right
	putPlace_[3].worldPos.x = col_ * tileSize_.x - (tileSize_.x / 2);
	putPlace_[3].worldPos.y = (row_ / 2) * tileSize_.y + (tileSize_.y / 2.0f);

	for (int i = 0; i < 4; i++) {
		putPlace_[i].localAdd = CalcCenterAdd(putPlace_[i].worldPos);
		putPlace_[i].worldAdd = CalcCenterAdd(putPlace_[i].worldPos);

		putPlace_[i].screenVertex = putPlace_[i].worldVertex;

		putPlace_[i].screenMatrix = putPlace_[i].worldMatrix;
	}

}


/*==========================================================
	更新処理関数
==========================================================*/
void Dog::Update() {

	// 犬を設置する準備
	Install();

	// 犬を置く処理
	Put();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

}


/*==========================================================
	描画処理関数
==========================================================*/
void Dog::Draw() {

	// 犬の描画
	if (isIdle_) {
		Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFFFF);

		for (int i = 0; i < 4; i++) {
			Draw::Quad(putPlace_[i].screenVertex, { 0.0f, 0.0f }, { 1.0f, 1.0f }, gh_, 0xA0522D77);
		}

	}

	DebugScreen();

}


/*==========================================================
	終了処理関数
==========================================================*/
void Dog::Finalize() {


}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Dog::Install() {
	if (!isIdle_) {
		if (input->IsTriggerKey(DIK_Z)) {
			isIdle_ = true;
		}

	} else {
		if (input->IsTriggerKey(DIK_Z)) {
			isIdle_ = false;
			isExist_ = false;

			isExistSide_ = NOT;

		}

		if (!isExist_) {
			worldCenterPos_ = {
				static_cast<float>(input->GetMousePos().x),
				static_cast<float>(input->GetMousePos().y)
			};
		}
	}
}

void Dog::Put() {
	if (isIdle_) {
		// マウスがクリックされた位置によって置く場所を決める
		if (input->IsTriggerMouse(0)) {
			for (int i = 0; i < 4; i++) {
				if (Collision::Rect::Point(putPlace_[i].screenVertex,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) }
				)) {

					worldCenterPos_ = putPlace_[i].worldPos;
					isExist_ = true;

					// 置いた場所を設定する
					switch (i) {
					case 0:
						isExistSide_ = TOP;
						break;

					case 1:
						isExistSide_ = BOTTOM;
						break;

					case 2:
						isExistSide_ = LEFT;
						break;

					case 3:
						isExistSide_ = RIGHT;
						break;
					}
				}
			}
		}
	}
}

Vec2 Dog::CalcCenterAdd(Vec2f centerPos) {
	Vec2 result;

	result.x = static_cast<int>(centerPos.x / tileSize_.x);
	result.y = static_cast<int>(centerPos.y / tileSize_.y);

	return result;

}

void Dog::DrawMatrixSelect() {
	if (isExist_) {
		drawMatrix_ = screenMatrix_;
	} else {
		drawMatrix_ = worldMatrix_;
	}
}

void Dog::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	// worldで表示するかscreenで表示するか選ぶ
	DrawMatrixSelect();

	screenVertex_.lt = Transform(localVertex_.lt, drawMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, drawMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, drawMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, drawMatrix_);

	for (int i = 0; i < 4; i++) {
		putPlace_[i].screenMatrix = MakeWvpVpMatrix(putPlace_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		putPlace_[i].screenVertex.lt = Transform(localVertex_.lt, putPlace_[i].screenMatrix);
		putPlace_[i].screenVertex.rt = Transform(localVertex_.rt, putPlace_[i].screenMatrix);
		putPlace_[i].screenVertex.lb = Transform(localVertex_.lb, putPlace_[i].screenMatrix);
		putPlace_[i].screenVertex.rb = Transform(localVertex_.rb, putPlace_[i].screenMatrix);
	}

}

void Dog::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

	for (int i = 0; i < 4; i++) {
		putPlace_[i].worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f }, 0.0f, putPlace_[i].worldPos);

		putPlace_[i].worldVertex.lt = Transform(localVertex_.lt, putPlace_[i].worldMatrix);
		putPlace_[i].worldVertex.rt = Transform(localVertex_.rt, putPlace_[i].worldMatrix);
		putPlace_[i].worldVertex.lb = Transform(localVertex_.lb, putPlace_[i].worldMatrix);
		putPlace_[i].worldVertex.rb = Transform(localVertex_.rb, putPlace_[i].worldMatrix);
	}
}

void Dog::DebugScreen() {
	for (int i = 0; i < 4; i++) {
		/*Novice::ScreenPrintf(900,
			600 + (i * 20),
			"putPlace_[%d].screenVertex.lt.y:%f",
			i,
			putPlace_[i].screenVertex.lt.y);*/
	}
}
