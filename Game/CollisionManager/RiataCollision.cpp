#include "RiataCollision.h"

RiataCollision::RiataCollision(
	Riata* riata,
	MapChip* mapChip,
	Cow* cow,
	Cowherd* cowherd,
	YoungPerson* youngPerson) {

	Init(riata, mapChip, cow, cowherd, youngPerson);
}

RiataCollision::~RiataCollision() { Finalize(); }


/*================================================
	初期化関数
================================================*/
void RiataCollision::Init(
	Riata* riata,
	MapChip* mapChip,
	Cow* cow,
	Cowherd* cowherd,
	YoungPerson* youngPerson) {

	riata_ = riata;
	mapChip_ = mapChip;
	cow_ = cow;
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;

}


/*================================================
	更新処理関数
================================================*/
void RiataCollision::Update() {

	// 輪投げが移動状態でなければ当たり判定を取らない
	if (!riata_->GetIsStart()) { return; }

	// 動きが止まる場所を探す
	//riata_->SetStartingPos()
	if (IsStop(riata_->GetWorldAddress())) {

	}

	// 今のアドレスとは別に目標地点をさがすためのアドレスを用意し、
	// 目標が定まったらそのアドレスをタイルサイズ文大きくし
	// 目標地点として設定しイージングなどをする

}



/*================================================
	終了処理関数
================================================*/
void RiataCollision::Finalize() {

	riata_ = nullptr;

	mapChip_ = nullptr;
	cow_ = nullptr;
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}


/*================================================
	その他メンバ関数
================================================*/

bool RiataCollision::IsStop(const Vec2& address) {
	// マップ上の障害物との判定
	if (mapChip_->GetMapChipAdd()[address.y][address.x] == ChipType::STAGEOUT) { return true; }
	if (mapChip_->GetMapChipAdd()[address.y][address.x] == ChipType::FENCE) { return true; }
	if (mapChip_->GetMapChipAdd()[address.y][address.x] == ChipType::ROCK) { return true; }

	// 牛との判定
	if (IsEqualAdd(address, cow_->GetCenterAdd())) {
		riata_->SetIsCatch(true);
		return true;
	}

	// 若人との判定
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {

		if (IsEqualAdd(address, youngPerson_->GetCenterAdd(yi))) { return true; }
	}

	return false;
}

bool RiataCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
