#include "MapChip.h"

MapChip::MapChip(){

	csvFilePath_[0] = "./Resources/stage/stageTest1.csv";

	mapAdd_ = LoadFile(csvFilePath_[0]);

	//行と列の初期化
	row_ = static_cast<int>(mapAdd_.size());
	col_ = static_cast<int>(mapAdd_[0].size());

	//配列の反転
	std::reverse(mapAdd_.begin(), mapAdd_.end());

	//配列の確保
	mapChip_ = new Base * [row_];
	for (int i = 0; i < row_; i++) {
		mapChip_[i] = new Base[col_];
	}

	Init();
}

MapChip::~MapChip(){
	// メモリを解放
	for (int i = 0; i < row_; i++) {
		delete[] mapChip_[i];
	}
	delete[] mapChip_;
}

//==============================================================
void MapChip::Init(){

	size_.x = 64.0f;
	size_.y = 64.0f;

	//マップの画像の番号の振り分け
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mapTile_[i][j].x = j * size_.x + (size_.x / 2);
			mapTile_[i][j].y = i * size_.y - (size_.y / 2);
		}
	}

	GH_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");

	//マップチップの初期化
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].pos.x = (size_.x * static_cast<float>(col)) + (size_.x / 2.0f);
			mapChip_[row][col].pos.y = (size_.y * static_cast<float>(row)) + (size_.y / 2.0f);

			mapChip_[row][col].scale.x = 1.0f;
			mapChip_[row][col].scale.y = 1.0f;

			
		}
	}

	//各頂点の初期化
	CalcWorldVertex();
	CalcScreenVertex();

	//原点中心vertex
	localVertex_.lt.x = -32.0f;
	localVertex_.lt.y = 32.0f;

	localVertex_.rt.x = 32.0f;
	localVertex_.rt.y = 32.0f;

	localVertex_.lb.x = -32.0f;
	localVertex_.lb.y = -32.0f;

	localVertex_.rb.x = 32.0f;
	localVertex_.rb.y = -32.0f;

}

//==============================================================

void MapChip::Update(){
}


//==============================================================
void MapChip::Draw(){
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] == STAGEOUT) {
				/*Novice::DrawSpriteRect(
					static_cast<int>(mapChip_[row][col].screenVertex.lt.x),
					static_cast<int>(mapChip_[row][col].screenVertex.lt.y),
					mapTileWidth_[0],
					mapTileHeight_[0],
					static_cast<int>(size_.x),
					static_cast<int>(size_.y),
					GH_,
					1,
					1,
					0.0f,
					0xFFFFFFFF
				);*/

				draw.Quad(mapChip_[row][col].screenVertex, mapTile_[0][0], size_, GH_, 0xFFFFFFFF);
			}
		}
	}
}


//==============================================================
void MapChip::CalcScreenVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].screenVertex.lt.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.lt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.rt.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.rt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.lb.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.lb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);

			mapChip_[row][col].screenVertex.rb.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].screenVertex.rb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::CalcWorldVertex() {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].worldVertex.lt.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.lt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.rt.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.rt.y = mapChip_[row][col].pos.y + (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.lb.x = mapChip_[row][col].pos.x - (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.lb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);

			mapChip_[row][col].worldVertex.rb.x = mapChip_[row][col].pos.x + (size_.x / 2.0f);
			mapChip_[row][col].worldVertex.rb.y = mapChip_[row][col].pos.y - (size_.y / 2.0f);
		}
	}
}

//==============================================================
void MapChip::MatrixChange(const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport) {
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			if (mapAdd_[row][col] != NONE) {
				worldMatrix_ = MakeAffineMatrix(mapChip_[row][col].scale, 0.0f, mapChip_[row][col].pos);

				screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, view, ortho, viewport);

				mapChip_[row][col].screenVertex.lt = Transform(localVertex_.lt, screenMatrix_);
				mapChip_[row][col].screenVertex.rt = Transform(localVertex_.rt, screenMatrix_);
				mapChip_[row][col].screenVertex.lb = Transform(localVertex_.lb, screenMatrix_);
				mapChip_[row][col].screenVertex.rb = Transform(localVertex_.rb, screenMatrix_);
			}
		}
	}
}

