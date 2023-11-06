#include "CowherdCollision.h"

CowherdCollision::CowherdCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow){
	Init(cowherd, youngPerson, mapChip, cow);
}

CowherdCollision::~CowherdCollision(){
	Finalize();
}

/*================================================================
	初期化関数
================================================================*/
void CowherdCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow){

	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;

}

/*================================================================
	終了処理関数
================================================================*/
void CowherdCollision::Finalize(){
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

/*================================================================
	更新処理関数
================================================================*/
void CowherdCollision::CowherdCanMove() {

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

bool CowherdCollision::CowherdCheckCanMove(const Vec2& add) {
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


bool CowherdCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
