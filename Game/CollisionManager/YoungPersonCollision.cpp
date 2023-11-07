#include "YoungPersonCollision.h"

YoungPersonCollision::YoungPersonCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow){
	Init(cowherd, youngPerson, mapChip, cow);
}

YoungPersonCollision::~YoungPersonCollision(){
	Finalize();
}

/*================================================================
	初期化関数
================================================================*/
void YoungPersonCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow){
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
}

/*================================================================
	終了処理関数
================================================================*/
void YoungPersonCollision::Finalize(){
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

/*================================================================
	更新処理関数
================================================================*/
void YoungPersonCollision::YoungPersonCanMove(){
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

bool YoungPersonCollision::YoungPersonCheckCanMove(const Vec2& add){
	// マップ上のオブジェクト
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::STAGEOUT) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::ROCK) { return false; }

	// 牛と牛飼い
	if (IsEqualAdd(add, cow_->GetCenterAdd())) { return false; }
	if (IsEqualAdd(add, cowherd_->GetCenterAdd())) { return false; }

	// 若人どうしの; 上下左右のアドレスを取って計算するので添え字が同じもの動詞でも計算して大丈夫
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}

	return true;
}

bool YoungPersonCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
