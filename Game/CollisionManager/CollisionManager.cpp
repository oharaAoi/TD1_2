#include "CollisionManager.h"

CollisionManager::CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	Init(cowherd, youngPerson, mapChip, cow);
}

CollisionManager::~CollisionManager() {
	Finalize();
}


/*================================================================
	初期化関数
================================================================*/
void CollisionManager::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
}


/*================================================================
	終了処理関数
================================================================*/
void CollisionManager::Finalize() {
	SafeDelete(mapChip_);
	SafeDelete(cowherd_);
	SafeDelete(youngPerson_);
	SafeDelete(cow_);
}

/*=================================================================
	実質更新処理
=================================================================*/
void CollisionManager::CheckCanMove() {

	CowherdCanMove();
	YoungPersonCanMove();
	CheckCowFourArea();
}


/*=================================================================
	牛飼い
=================================================================*/

void CollisionManager::CowherdCanMove() {

	// 中心アドレスから上下左右、斜めの番地を確認
	// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない
	// 障害物がなければ移動できる

	if (cowherd_->GetIsMoveIdle()) {
		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x, cowherd_->GetCenterAdd().y + 1 })) { // 上
			cowherd_->SetCanMoveDir(true, top);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x, cowherd_->GetCenterAdd().y - 1 })) { // 下
			cowherd_->SetCanMoveDir(true, bottom);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y })) { // 左
			cowherd_->SetCanMoveDir(true, left);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y })) { // 右
			cowherd_->SetCanMoveDir(true, right);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y + 1 })) { // 左上
			cowherd_->SetCanMoveDir(true, leftTop);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y + 1 })) { // 右上
			cowherd_->SetCanMoveDir(true, rightTop);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y - 1 })) { // 左下
			cowherd_->SetCanMoveDir(true, leftBottom);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y - 1 })) { // 右下
			cowherd_->SetCanMoveDir(true, rightBottom);
		}
	}


}

bool CollisionManager::CowherdCheckCanMove(const Vec2& add) {
	// 動かない物はアドレス上で当たり判定を取る
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }

	// 動くものはアドレスが被っていないかで
	for (int yi = 0; yi < youngPerson_->GetIndexMax(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}

	return false;
}



/*=================================================================
	若人
=================================================================*/

void CollisionManager::YoungPersonCanMove() {

	for (int yi = 0; yi < youngPerson_->GetIndexMax(); yi++) {

		if (youngPerson_->GetIsMoveIdle(yi)) {

			// 中心アドレスから上下左右、斜めの番地を確認
			// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない
			// 障害物がなければ移動できる

			//if(YoungPersonCheckCanMove(youngPerson_->GetCenterAdd()))

		}

	}

}

bool CollisionManager::YoungPersonCheckCanMove(const Vec2& add) {
	add;
	return false;
}

/*=================================================================
	牛
=================================================================*/

void  CollisionManager::CheckCowFourArea() {
	// 左上を調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis < mapChip_->GetMapChipRow(); yAxis++) {
		for (int xAxis = 0; xAxis < cow_->GetCenterAdd().x; xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis){
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - 2, kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - 2, kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - 2, kCanMoveDirection::top);
			}

			// 若人
		}
	}
}


bool CollisionManager::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
