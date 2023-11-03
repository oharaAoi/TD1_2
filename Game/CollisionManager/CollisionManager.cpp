#include "CollisionManager.h"

CollisionManager::CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip) {
	Init(cowherd, youngPerson, mapChip);
}

CollisionManager::~CollisionManager() {
	Finalize();
}


/*================================================================
	初期化関数
================================================================*/
void CollisionManager::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
}


/*================================================================
	終了処理関数
================================================================*/
void CollisionManager::Finalize() {
	SafeDelete(mapChip_);
	SafeDelete(cowherd_);
	SafeDelete(youngPerson_);
}

void CollisionManager::CowherdCanMove() {

	// 中心アドレスから上下左右、斜めの番地を確認
	//if(cowherd_->GetCenterAdd().x)

	// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない

	// 障害物がなければ移動できる

}

bool CollisionManager::CowherdCheckCanMove(const Vec2& add) {
	// 動かない物はアドレス上で当たり判定を取る
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }

	// 動くものはアドレスが被っていないかで
	//if(IsEqualAdd(add, ))

	return false;
}

bool CollisionManager::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
