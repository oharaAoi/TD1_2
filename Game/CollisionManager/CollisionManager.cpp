#include "CollisionManager.h"

CollisionManager::CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip,
	Cow* cow, Dog* dog, BullCow* bull, BullFighting* fighting) {
	Init(cowherd, youngPerson, mapChip, cow, dog, bull, fighting);
}

CollisionManager::~CollisionManager() {
	Finalize();
}


/*================================================================
	初期化関数
================================================================*/
void CollisionManager::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip,
	Cow* cow, Dog* dog, BullCow* bull, BullFighting* fighting) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
	dog_ = dog;
	bull_ = bull;
	fighting_ = fighting;

	cowCollision_ = new CowCollision(cowherd_, youngPerson_, mapChip_, cow_, dog_);
	cowherdCollison_ = new CowherdCollision(cowherd_, youngPerson_, mapChip_, cow_);
	youngPersonCollision_ = new YoungPersonCollision(cowherd_, youngPerson_, mapChip_, cow_);
	bullCollision_ = new BullCollision(cowherd_, youngPerson_, mapChip_, bull_, dog_);
	fightingCollision_ = new FightingCollision(cowherd_, youngPerson_, mapChip_, fighting_, dog_);
}


/*================================================================
	終了処理関数
================================================================*/
void CollisionManager::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
	mapChip_ = nullptr;
	cow_ = nullptr;
	dog_ = nullptr;
	bull_ = nullptr;
	SafeDelete(cowCollision_);
	SafeDelete(cowherdCollison_);
	SafeDelete(youngPersonCollision_);
	SafeDelete(bullCollision_);
	SafeDelete(fightingCollision_);
}

/*=================================================================
	実質更新処理
=================================================================*/
void CollisionManager::CheckCanMove() {

	cowherdCollison_->CowherdCanMove();
	
	youngPersonCollision_->YoungPersonCanMove();

	/*cowCollision_->CheckCowMoveDire();*/
	/*YoungPersonCanMove();*/
	
}

/* --- デバック用 --- */
void CollisionManager::CheckCanCowMove() {
	cowCollision_->CheckCowMoveDire();
}

void CollisionManager::CheckCanBullMove() {
	bullCollision_->CheckBullMoveDire();
}

void CollisionManager::CheckCanFigthingMove(){
	fightingCollision_->CheckCanMoveDire();
}

/* --- 両隣がステージ外か上下がステージ外だったら*/
void CollisionManager::CheckCowCollison() {
	cowCollision_->CheckFenseScissorsCollision();
	cowCollision_->CheckRockCollision();
}

void CollisionManager::CheckBullCollision() {
	bullCollision_->CheckFenceScissorsCollision();
	bullCollision_->CheckRockCollision();
}



/* --- クリア判定 --- */
bool CollisionManager::CheckClear() {
	// 牛飼いと牛のアドレスが重なっていたら
	if (cowherd_->GetCenterAdd().x == cow_->GetCenterAdd().x &&
		cowherd_->GetCenterAdd().y == cow_->GetCenterAdd().y) {

		return true;

	} else {
		return false;
	}
}

/* ---重なっているか --- */
// 牛のいる位置がNONE以外だったら前の座標に戻す
void CollisionManager::CheckOverLapping() {
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x] != ChipType::NONE) {
		cow_->SetWorldCenterPos(cow_->GetPreCenterPos());
	}
}


bool CollisionManager::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
