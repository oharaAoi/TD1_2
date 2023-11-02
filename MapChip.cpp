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

	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {
			mapChip_[row][col].pos.x = (size_.x * static_cast<float>(col)) + (size_.x / 2.0f);
			mapChip_[row][col].pos.y = (size_.y * static_cast<float>(row)) + (size_.y / 2.0f);

			mapChip_[row][col].scall.x = 1.0f;
			mapChip_[row][col].scall.y = 1.0f;

			
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
