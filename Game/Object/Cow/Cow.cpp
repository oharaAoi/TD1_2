#include "Cow.h"

Cow::Cow(MapChip* mapChip) { Init(mapChip); }

Cow::~Cow() { Finalize(); }


/*==========================================================
	初期化関数
==========================================================*/
void Cow::Init(MapChip* mapChip){

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

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// アドレスの初期化
	CenterAddUpdate();
}


/*==========================================================
	更新処理関数
==========================================================*/
void Cow::Update(){

	// 牛の現在の位置を取得
	CenterAddUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

	// 牛の方向をリセットする
	DireInit();
}


/*==========================================================
	描画処理関数
==========================================================*/
void Cow::Draw(){

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

void Cow::CheckNearPerson(){
	for (int i = 0; i < 8; i++) {
		/*gridDistanceValue_[i] = std::clamp(gridDistanceValue_[i], 0, 10);*/
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
