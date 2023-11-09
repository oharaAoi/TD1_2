﻿#include "YoungPersonCollision.h"

YoungPersonCollision::YoungPersonCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	Init(cowherd, youngPerson, mapChip, cow);
}

YoungPersonCollision::~YoungPersonCollision() {
	Finalize();
}

/*================================================================
	初期化関数
================================================================*/
void YoungPersonCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
}

/*================================================================
	終了処理関数
================================================================*/
void YoungPersonCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
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
