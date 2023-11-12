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

	// 輪投げが移動待機状態でなければ当たり判定を取らない
	if (!riata_->GetIsIdle()) { return; }

	// 動きが止まる場所を探す
	//riata_->SetStartingPos()


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
