#include "RiataCollision.h"

RiataCollision::RiataCollision(
	Riata* riata,
	MapChip* mapChip,
	Cow* cow,
	BullCow* bullCow,
	BullFighting* bullFighting,
	Cowherd* cowherd,
	YoungPerson* youngPerson) {

	Init(riata, mapChip, cow, bullCow, bullFighting, cowherd, youngPerson);
}

RiataCollision::~RiataCollision() { Finalize(); }


/*================================================
	初期化関数
================================================*/
void RiataCollision::Init(
	Riata* riata,
	MapChip* mapChip,
	Cow* cow,
	BullCow* bullCow,
	BullFighting* bullFighting,
	Cowherd* cowherd,
	YoungPerson* youngPerson) {

	riata_ = riata;
	mapChip_ = mapChip;
	cow_ = cow;
	bullCow_ = bullCow;
	bullFighting_ = bullFighting;
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;

}


/*================================================
	更新処理関数
================================================*/
void RiataCollision::Update() {

	// 輪投げが移動状態でなければ当たり判定を取らない
	if (!riata_->GetIsIdle()) { return; }
	if (!riata_->GetIsStart()) { return; }
	//if (riata_->GetIsStart()) { return; }
	if (riata_->GetMoveDir().x == 0 && riata_->GetMoveDir().y == 0) { return; }

	// 動きが止まる場所を探す
	//riata_->SetStartingPos()
	Vec2 address = riata_->GetWorldAddress();
	while (!IsStop(address)) {
		address += {static_cast<int>(riata_->GetMoveDir().x), static_cast<int>(riata_->GetMoveDir().y)};
	}

	riata_->SetDestinationPos(
		Vec2f{ static_cast<float>(address.x), static_cast<float>(address.y) } *mapChip_->GetTileSize()
		+ Vec2f{ mapChip_->GetTileSize().x * 0.5f , mapChip_->GetTileSize().y * 0.5f }
	);

	//riata_->SetIsStart(true);

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
	bullCow_ = nullptr;
	bullFighting_ = nullptr;
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
	if (IsEqualAdd(address, bullCow_->GetCenterAdd())) {
		riata_->SetIsCatch(true);
		return true;
	}
	if (IsEqualAdd(address, bullFighting_->GetWorldAdd())) {
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
