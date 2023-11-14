#include "YoungPersonCollision.h"

YoungPersonCollision::YoungPersonCollision(Cowherd* cowherd,
	YoungPerson* youngPerson,
	MapChip* mapChip,
	Cow* cow,
	BullCow* bullCow,
	BullFighting* fighting) {
	Init(cowherd, youngPerson, mapChip, cow, bullCow, fighting);
}

YoungPersonCollision::~YoungPersonCollision() {
	Finalize();
}

/*================================================================
	初期化関数
================================================================*/
void YoungPersonCollision::Init(Cowherd* cowherd,
	YoungPerson* youngPerson,
	MapChip* mapChip,
	Cow* cow,
	BullCow* bullCow,
	BullFighting* fighting) {

	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
	bullCow_ = bullCow;
	fighting_ = fighting;
}

/*================================================================
	終了処理関数
================================================================*/
void YoungPersonCollision::Finalize() {

	cowherd_ = nullptr;
	youngPerson_ = nullptr;
	mapChip_ = nullptr;
	cow_ = nullptr;
	bullCow_ = nullptr;
	fighting_ = nullptr;

}

/*================================================================
	更新処理関数
================================================================*/
void YoungPersonCollision::YoungPersonCanMove() {
	// 若者の配列
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		// 移動マスの配列
		for (int gi = 0; gi < youngPerson_->GetCanMoveGridMaxIndex(); gi++) {

			// 移動待機状態
			if (youngPerson_->GetIsMoveIdle(yi)) {

				// 移動できるかチェック
				if (YoungPersonCheckCanMove(youngPerson_->GetCanMoveGrid(yi)[gi].worldAdd)) {

					youngPerson_->SetCanMove(true, yi, gi);

				} else {

					youngPerson_->SetCanMove(false, yi, gi);
				}
			}

		}

		// 障害物の飛び越え禁止 --------------------------------------------------------
		// 移動マスの配列
		for (int gi = 0; gi < youngPerson_->GetCanMoveGridMaxIndex(); gi++) {
			// プレイヤーの中心アドレスから上の移動マス;
			if (youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.x == 0
				&& youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.y == 1) {

				// 移動できない
				if (!youngPerson_->GetCanMoveGrid(yi)[gi].canMove) {

					// 移動マスの配列
					for (int gj = 0; gj < youngPerson_->GetCanMoveGridMaxIndex(); gj++) {

						// 上で探したマスの1つ上
						if (youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.x == 0
							&& youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.y == 2) {

							// 移動できないようにSet
							youngPerson_->SetCanMove(false, yi, gj);

						}
					}
				}
			}

			// プレイヤーの中心アドレスから下の移動マス;
			if (youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.x == 0
				&& youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.y == -1) {

				// 移動できない
				if (!youngPerson_->GetCanMoveGrid(yi)[gi].canMove) {

					// 移動マスの配列
					for (int gj = 0; gj < youngPerson_->GetCanMoveGridMaxIndex(); gj++) {

						// 上で探したマスの左隣
						if (youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.x == 0
							&& youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.y == -2) {

							// 移動できないようにSet
							youngPerson_->SetCanMove(false, yi, gj);

						}
					}
				}
			}


			// プレイヤーの中心アドレスから左の移動マス;
			if (youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.x == -1
				&& youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.y == 0) {

				// 移動できない
				if (!youngPerson_->GetCanMoveGrid(yi)[gi].canMove) {

					// 移動マスの配列
					for (int gj = 0; gj < youngPerson_->GetCanMoveGridMaxIndex(); gj++) {

						// 上で探したマスの左隣
						if (youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.x == -2
							&& youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.y == 0) {

							// 移動できないようにSet
							youngPerson_->SetCanMove(false, yi, gj);

						}
					}
				}
			}

			// プレイヤーの中心アドレスから右の移動マス;
			if (youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.x == 1
				&& youngPerson_->GetCanMoveGrid(yi)[gi].localAdd.y == 0) {

				// 移動できない
				if (!youngPerson_->GetCanMoveGrid(yi)[gi].canMove) {

					// 移動マスの配列
					for (int gj = 0; gj < youngPerson_->GetCanMoveGridMaxIndex(); gj++) {

						// 上で探したマスの左隣
						if (youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.x == 2
							&& youngPerson_->GetCanMoveGrid(yi)[gj].localAdd.y == 0) {

							// 移動できないようにSet
							youngPerson_->SetCanMove(false, yi, gj);

						}
					}
				}
			}
		}


	}


}

bool YoungPersonCollision::YoungPersonCheckCanMove(const Vec2& add) {
	// マップ上のオブジェクト
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::STAGEOUT) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::ROCK) { return false; }

	// 牛
	if (IsEqualAdd(add, cow_->GetCenterAdd())) { return false; }
	if (IsEqualAdd(add, bullCow_->GetCenterAdd())) { return false; }
	if (IsEqualAdd(add, fighting_->GetWorldAdd())) { return false; }

	// 牛飼い
	if (IsEqualAdd(add, cowherd_->GetCenterAdd())) { return false; }

	// 若人どうしの; 上下左右のアドレスを取って計算するので添え字が同じもの動詞でも計算して大丈夫
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}

	return true;
}

/*================================================================
	当たり判定
================================================================*/

void YoungPersonCollision::YoungPresonIsStriked() {
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (youngPerson_->GetIsStriked(yi) == true) {
			switch (fighting_->GetMovedDire()) {
			case kCanMoveDirection::top:
				youngPerson_->SetWorldPos({
					youngPerson_->GetWorldPos(yi).x, youngPerson_->GetWorldPos(yi).y + mapChip_->GetTileSize().y }, yi
					);

				youngPerson_->SetIsStan(true, yi);
				youngPerson_->SetIsStriked(false, yi);
				break;

			case kCanMoveDirection::bottom:
				youngPerson_->SetWorldPos({
					youngPerson_->GetWorldPos(yi).x, youngPerson_->GetWorldPos(yi).y - mapChip_->GetTileSize().y }, yi
					);

				youngPerson_->SetIsStan(true, yi);
				youngPerson_->SetIsStriked(false, yi);
				break;

			case kCanMoveDirection::left:
				youngPerson_->SetWorldPos({
					youngPerson_->GetWorldPos(yi).x - mapChip_->GetTileSize().x , youngPerson_->GetWorldPos(yi).y }, yi
					);

				youngPerson_->SetIsStan(true, yi);
				youngPerson_->SetIsStriked(false, yi);
				break;

			case kCanMoveDirection::right:
				youngPerson_->SetWorldPos({
					youngPerson_->GetWorldPos(yi).x + mapChip_->GetTileSize().x , youngPerson_->GetWorldPos(yi).y }, yi
					);

				youngPerson_->SetIsStan(true, yi);
				youngPerson_->SetIsStriked(false, yi);
				break;
			}
		}
	}
}

bool YoungPersonCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
