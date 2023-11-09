#include "FightingCollision.h"

FightingCollision::FightingCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog){
	Init(cowherd, youngPerson, mapChip, fighting, dog);
}

FightingCollision::~FightingCollision() { Finalize(); }

void FightingCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

/*========================================================
	初期化関数
========================================================*/
void FightingCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog){
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	fighting_ = fighting;
	dog_ = dog;
}


/*========================================================
	更新処理関数
========================================================*/


/*========================================================
	評価
========================================================*/


/*==========================================================================================================
										岩との評価
============================================================================================================*/
void FightingCollision::CheckRockAdjoin(){
}

/*==========================================================================================================
										進行方向の評価(csvでやる)
============================================================================================================*/
void FightingCollision::CheckMoveDire(){
}

/*==========================================================================================================
										4つのエリアでの評価
============================================================================================================*/
void FightingCollision::CheckFourAreas(){
}