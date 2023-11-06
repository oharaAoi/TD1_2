#include "CollisionManager.h"

CollisionManager::CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip,
	Cow* cow, Dog* dog, CowCollision* cowCollision_) {
	Init(cowherd, youngPerson, mapChip, cow, dog, cowCollision_);
}

CollisionManager::~CollisionManager() {
	Finalize();
}


/*================================================================
	初期化関数
================================================================*/
void CollisionManager::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip,
	Cow* cow, Dog* dog , CowCollision* cowCollision) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
	dog_ = dog;
	cowCollision_ = cowCollision;
}


/*================================================================
	終了処理関数
================================================================*/
void CollisionManager::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

/*=================================================================
	実質更新処理
=================================================================*/
void CollisionManager::CheckCanMove() {

	CowherdCanMove();
	YoungPersonCanMove();
	CheckCanCowMove();
}

void CollisionManager::CheckCanCowMove() {
	cowCollision_->CheckCowMoveDire();
}

/*=================================================================
	牛飼い
=================================================================*/

void CollisionManager::CowherdCanMove() {

	// 中心アドレスから上下左右、斜めの番地を確認
	// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない
	// 障害物がなければ移動できる

	if (cowherd_->GetIsMoveIdle()) {

		for (int gi = 0; gi < cowherd_->GetCanMoveGirdMaxIndex(); gi++) {

			// 移動待機状態
			cowherd_->SetCanMove(false, gi);
			if (cowherd_->GetIsMoveIdle()) {

				// 移動できるかチェック
				if (CowherdCheckCanMove(cowherd_->GetCanMoveGrid()[gi].worldAdd)) {

					cowherd_->SetCanMove(true, gi);

				}

			}

		}

	}


}

bool CollisionManager::CowherdCheckCanMove(const Vec2& add) {
	// 動かない物はアドレス上で当たり判定を取る
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::STAGEOUT) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::ROCK) { return false; }

	// 動くものはアドレスが被っていないかで
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}

	return true;
}

bool CollisionManager::CheckClear() {
	// 牛飼いと牛のアドレスが重なっていたら
	if (cowherd_->GetCenterAdd().x == cow_->GetCenterAdd().x &&
		cowherd_->GetCenterAdd().y == cow_->GetCenterAdd().y) {

		return true;

	} else {
		return false;
	}
}

/*=================================================================
	若人
=================================================================*/

void CollisionManager::YoungPersonCanMove() {

	// 若者の配列
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		// 移動マスの配列
		for (int gi = 0; gi < youngPerson_->GetCanMoveGridMaxIndex(); gi++) {

			// 移動待機状態
			youngPerson_->SetCanMove(false, yi, gi);
			if (youngPerson_->GetIsMoveIdle(yi)) {

				// 移動できるかチェック
				if (YoungPersonCheckCanMove(youngPerson_->GetCanMoveGrid(yi)[gi].worldAdd)) {

					youngPerson_->SetCanMove(true, yi, gi);

				}

			}

		}

	}

}

bool CollisionManager::YoungPersonCheckCanMove(const Vec2& add) {
	// マップ上のオブジェクト
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::STAGEOUT) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::ROCK) { return false; }

	// 牛との

	// 牛飼いとの
	if (IsEqualAdd(add, cowherd_->GetCenterAdd())) { return false; }

	// 若人どうしの; 上下左右のアドレスを取って計算するので添え字が同じもの動詞でも計算して大丈夫
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}

	return true;
}

/*=================================================================
	牛
=================================================================*/

// 牛のいる位置がNONE以外だったら前の座標に戻す
void CollisionManager::CheckOverLapping() {
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x] != ChipType::NONE) {
		cow_->SetWorldCenterPos(cow_->GetPreCenterPos());
	}
}


bool CollisionManager::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
