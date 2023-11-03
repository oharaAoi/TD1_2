#include "BaseMap.h"

//===========================================================================
//　静的メンバ変数の初期化
std::string BaseMap::csvFilePath_ = "./Resources/stage/stageTest1.csv";
std::vector<std::vector<int>> BaseMap::mapAdd_ = LoadFile(csvFilePath_);

int BaseMap::row_ = static_cast<int>(mapAdd_.size());
int BaseMap::col_ = static_cast<int>(mapAdd_[0].size());

Vec2f BaseMap::tileSize_ = { 64.0f, 64.0f };

int BaseMap::GH_ = 0;

//===========================================================================

BaseMap::BaseMap() {
	AddressRevers();
	Init();
}

BaseMap::~BaseMap() {}

//===========================================================================
void BaseMap::AddressRevers(){
	std::reverse(mapAdd_.begin(), mapAdd_.end());
}

//===========================================================================
void BaseMap::Init(){
	GH_ = Novice::LoadTexture("./Resources/images/mapTile/colorMap.png");
}


