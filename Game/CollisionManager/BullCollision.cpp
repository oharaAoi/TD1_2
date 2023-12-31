﻿#include "BullCollision.h"

BullCollision::BullCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog) {
	Init(cowherd, youngPerson, mapChip, bullCow, dog);
}

BullCollision::~BullCollision() { Finalize(); }

void BullCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

void BullCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	bullCow_ = bullCow;
	dog_ = dog;
}

void BullCollision::CheckBullMoveDire() {
	bullCow_->DireInit();

	// 犬がいる時
	CheckDogExist();

	// 両隣フェンスの時
	CheckBullCowAdjoin();

	// マス目の計算(牧師
	CheckGridDistance(cowherd_->GetCenterAdd());

	// 範囲の計算
	CheckGridDire(cowherd_->GetCenterAdd());

	//// マス目の計算(若人
	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckGridDistance(youngPerson_->GetCenterAdd(i));

		CheckGridDire(youngPerson_->GetCenterAdd(i));
	}

	for (int row = 0; row < mapChip_->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip_->GetMapChipCol(); col++) {
			if (mapChip_->GetMapChipAdd()[row][col] == ChipType::ROCK) {
				Vec2 rockAdd = { col, row };
				CheckGridDire(rockAdd);
			}
		}
	}

	// 4つのエリア
	CheckFourAreas();

	// 壁
	CheckNearWall();

	// 牛から見た全方向
	CheckBullCowMoveAllDire();

	CheckFenseCollision();

	CheckSameDireValue();

}

/*==========================================================================================================
										岩と重なってしまったら
============================================================================================================*/
void BullCollision::CheckRockCollision() {
	if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x] == ChipType::ROCK) {

		switch (bullCow_->GetMovedDire()) {
		case kCanMoveDirection::top:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);
			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::bottom:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);
			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::left:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::right:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::leftTop:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::rightTop:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::leftBottom:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::rightBottom:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);
			bullCow_->SetIsMove(false);
			break;
		}
	}
}

/*==========================================================================================================
										フェンスと重なってしまったら
============================================================================================================*/

void BullCollision::CheckFenceScissorsCollision() {
	if ((mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x] == ChipType::STAGEOUT &&
		mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x] == ChipType::STAGEOUT)) {

		switch (bullCow_->GetMovedDire()) {
		case kCanMoveDirection::top:

			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsFenceAttack(true);
			bullCow_->SetIsMove(false);

			mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			break;

		case kCanMoveDirection::bottom:

			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsFenceAttack(true);
			bullCow_->SetIsMove(false);

			mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			break;

		case kCanMoveDirection::left:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);

			bullCow_->SetIsFenceAttack(true);
			bullCow_->SetIsMove(false);

			mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			break;

		case kCanMoveDirection::right:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			bullCow_->SetIsFenceAttack(true);
			bullCow_->SetIsMove(false);

			mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			break;
		}

	} else {

		// 両隣外かフェンスだったら
		if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x + 1] == ChipType::FENCE &&
			mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x - 1] == ChipType::FENCE) {

			// 真ん中により上だったら下に戻す
			if (bullCow_->GetCenterAdd().y > mapChip_->GetMapChipRow() / 2.0f) {
				bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
					}
				);

				bullCow_->SetIsFenceAttack(true);
				bullCow_->SetIsMove(false);

				mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());

				// 真ん中により下だったら上に戻す
			} else if (bullCow_->GetCenterAdd().y < mapChip_->GetMapChipRow() / 2.0f) {
				bullCow_->SetWorldPos({
					(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
					(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
					}
				);
				bullCow_->SetIsFenceAttack(true);
				bullCow_->SetIsMove(false);

				mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			}
		}

		// 上下がフェンスだったら
		if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y + 1][bullCow_->GetCenterAdd().x] == ChipType::FENCE &&
			mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y - 1][bullCow_->GetCenterAdd().x] == ChipType::FENCE) {

			// 真ん中により右だったら左に戻す
			if (bullCow_->GetCenterAdd().x > mapChip_->GetMapChipCol() / 2.0f) {
				bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
					}
				);
				bullCow_->SetIsFenceAttack(true);
				bullCow_->SetIsMove(false);

				mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());

				// 真ん中により左だったら右に戻す
			} else if (bullCow_->GetCenterAdd().x < mapChip_->GetMapChipCol() / 2.0f) {
				bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
					}
				);
				bullCow_->SetIsFenceAttack(true);
				bullCow_->SetIsMove(false);

				mapChip_->SetFenceHp(mapChip_->GetFenceHp(bullCow_->GetCenterAdd()) - 1, bullCow_->GetCenterAdd());
			}

		}
	}
}

/*==========================================================================================================
										playerと重なってしまったら
============================================================================================================*/
void BullCollision::PersonCollision(const Vec2& add) {
	if (IsEqualAdd(bullCow_->GetCenterAdd(), add)) {

		switch (bullCow_->GetMovedDire()) {
		case kCanMoveDirection::top:

			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::bottom:

			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);

			bullCow_->SetIsMove(false);
			break;

		case kCanMoveDirection::left:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) + mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);

			bullCow_->SetIsMove(false);

			break;

		case kCanMoveDirection::right:
			bullCow_->SetWorldPos({
				(bullCow_->GetCenterAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f) - mapChip_->GetTileSize().x,
				(bullCow_->GetCenterAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f),
				}
			);

			bullCow_->SetIsMove(false);

			break;
		}

	}
}

/*==========================================================================================================
										フェンスと隣あっていたら
============================================================================================================*/
void BullCollision::CheckFenseCollision() {
	// 処理の順番的にここで書き換えないと影響しない(cowのUpdateに入る前にここにはいるから)
	// フェンスに攻撃したらフラグがtrueのため評価値を小さくして置く(そのターンは攻撃しない)
	if (bullCow_->GetIsFenseAttack() == true) {
		bullCow_->SetFenceValue(-200);
	} else {
		// cowの前ループ処理でtrueの時はfalseにしているため攻撃して移動しその次には値が+なるようにする
		bullCow_->SetFenceValue(200);
	}

	// top
	if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y + 1][bullCow_->GetCenterAdd().x] == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetFenceValue(), kCanMoveDirection::top);
	}

	// bottom
	if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y - 1][bullCow_->GetCenterAdd().x] == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetFenceValue(), kCanMoveDirection::bottom);
	}

	// left
	if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x - 1] == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetFenceValue(), kCanMoveDirection::left);
	}

	// right
	if (mapChip_->GetMapChipAdd()[bullCow_->GetCenterAdd().y][bullCow_->GetCenterAdd().x + 1] == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetFenceValue(), kCanMoveDirection::right);
	}


	/* ------ 斜めの場合は値を引く ------- */
	// leftTop
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y + 1, bullCow_->GetCenterAdd().x - 1) == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetSlantFenceValue(), kCanMoveDirection::leftTop);
	}

	// rightTop
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y + 1, bullCow_->GetCenterAdd().x + 1) == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetSlantFenceValue(), kCanMoveDirection::rightTop);
	}

	// leftBottom
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y - 1, bullCow_->GetCenterAdd().x - 1) == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetSlantFenceValue(), kCanMoveDirection::leftBottom);
	}

	// rightBottom
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y - 1, bullCow_->GetCenterAdd().x + 1) == ChipType::FENCE) {
		bullCow_->SetMoveDireValue(bullCow_->GetSlantFenceValue(), kCanMoveDirection::rightBottom);
	}

}

/*==========================================================================================================
													犬がいる時
============================================================================================================*/
void BullCollision::CheckDogExist() {
	// 犬がいる方向を入手しいる場合は値を減らす
	switch (dog_->GetIsExistSide()) {
	case ExsitSide::TOP:
		bullCow_->SetMoveDireValue(bullCow_->GetDogValue(), kCanMoveDirection::bottom);
		break;

	case ExsitSide::BOTTOM:
		bullCow_->SetMoveDireValue(bullCow_->GetDogValue(), kCanMoveDirection::top);
		break;

	case ExsitSide::LEFT:
		bullCow_->SetMoveDireValue(bullCow_->GetDogValue(), kCanMoveDirection::right);
		break;

	case ExsitSide::RIGHT:
		bullCow_->SetMoveDireValue(bullCow_->GetDogValue(), kCanMoveDirection::left);
		break;
	}
}

/*==========================================================================================================
												一番近い壁を計算
============================================================================================================*/
void BullCollision::CheckNearWall() {
	int nearWallOfValue[4]{};

	nearWallOfValue[kCanMoveDirection::top] = mapChip_->GetMapChipRow() - bullCow_->GetCenterAdd().y - 1;
	nearWallOfValue[kCanMoveDirection::bottom] = bullCow_->GetCenterAdd().y;

	nearWallOfValue[kCanMoveDirection::left] = bullCow_->GetCenterAdd().x;
	nearWallOfValue[kCanMoveDirection::right] = mapChip_->GetMapChipCol() - bullCow_->GetCenterAdd().x - 1;

	int wallMin_ = 1000;
	int wallMinIndex_ = 0;
	int wallMinNum_ = 0;

	// 同値があった時のフラグ
	bool wallIsDuplicate_ = false;

	// 一番低い値を求める
	for (int i = 0; i < 4; i++) {
		if (wallMin_ > nearWallOfValue[i]) {
			wallMin_ = nearWallOfValue[i];
			wallMinIndex_ = i;

		} else if (wallMin_ == nearWallOfValue[i]) {
			wallMinNum_++;
			wallIsDuplicate_ = true;
		}
	}

	// 同値がなかった時はそれぞれの辺だけで済む
	if (!wallIsDuplicate_) {
		switch (wallMinIndex_) {
		case kCanMoveDirection::top:
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetWallValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetWallValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetWallValue(), kCanMoveDirection::rightTop);

			break;

		case kCanMoveDirection::bottom:
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetWallValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetWallValue(), kCanMoveDirection::leftBottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetWallValue(), kCanMoveDirection::rightBottom);
			break;

		case kCanMoveDirection::left:
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetWallValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetWallValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetWallValue(), kCanMoveDirection::leftBottom);
			break;

		case kCanMoveDirection::right:
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetWallValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetWallValue(), kCanMoveDirection::rightTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetWallValue(), kCanMoveDirection::rightBottom);
			break;
		}
	}

	/// 2つ同値があった時
	if (wallMinNum_ == 2) {
		if (nearWallOfValue[kCanMoveDirection::top] == nearWallOfValue[kCanMoveDirection::right] &&
			nearWallOfValue[kCanMoveDirection::top] == nearWallOfValue[kCanMoveDirection::left]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetWallValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetWallValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetWallValue(), kCanMoveDirection::rightTop);

		} else if (nearWallOfValue[kCanMoveDirection::bottom] == nearWallOfValue[kCanMoveDirection::left] &&
			nearWallOfValue[kCanMoveDirection::bottom] == nearWallOfValue[kCanMoveDirection::right]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetWallValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetWallValue(), kCanMoveDirection::leftBottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetWallValue(), kCanMoveDirection::rightBottom);

		} else if (nearWallOfValue[kCanMoveDirection::left] == nearWallOfValue[kCanMoveDirection::top] &&
			nearWallOfValue[kCanMoveDirection::left] == nearWallOfValue[kCanMoveDirection::bottom]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetWallValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetWallValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetWallValue(), kCanMoveDirection::leftBottom);

		} else if (nearWallOfValue[kCanMoveDirection::right] == nearWallOfValue[kCanMoveDirection::top] &&
			nearWallOfValue[kCanMoveDirection::right] == nearWallOfValue[kCanMoveDirection::bottom]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetWallValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetWallValue(), kCanMoveDirection::rightTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetWallValue(), kCanMoveDirection::rightBottom);

		}

	} else if (wallMinNum_ == 1) {
		if (nearWallOfValue[kCanMoveDirection::top] == nearWallOfValue[kCanMoveDirection::left]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetWallValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetWallValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetWallValue(), kCanMoveDirection::leftTop);

		} else if (nearWallOfValue[kCanMoveDirection::top] == nearWallOfValue[kCanMoveDirection::right]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetWallValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetWallValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetWallValue(), kCanMoveDirection::rightTop);

		} else if (nearWallOfValue[kCanMoveDirection::bottom] == nearWallOfValue[kCanMoveDirection::left]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetWallValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetWallValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetWallValue(), kCanMoveDirection::leftBottom);

		} else if (nearWallOfValue[kCanMoveDirection::bottom] == nearWallOfValue[kCanMoveDirection::right]) {

			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetWallValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetWallValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetWallValue(), kCanMoveDirection::rightBottom);
		}
	}

}

/*==========================================================================================================
												マス目ごとの計算
============================================================================================================*/
void BullCollision::CheckGridDistance(const Vec2& add) {
	Vec2f cow2PlayerDis{};
	Vec2 naturalDis{};
	int directionValue[8]{ 0 };
	int keepDire[8]{ 0 };

	for (int i = 0; i < 8; i++) {
		/*directionValue[i] = cow_->GetGridDistanceValue(i);*/
	}

	// 牛飼いとのマス目上の距離を取る
	cow2PlayerDis.x = static_cast<float>(bullCow_->GetCenterAdd().x - add.x);
	cow2PlayerDis.y = static_cast<float>(bullCow_->GetCenterAdd().y - add.y);

	// 値を入れるときように-のものを+にする
	naturalDis.x = static_cast<int>(sqrtf(powf(cow2PlayerDis.x, 2.0f)));
	naturalDis.y = static_cast<int>(sqrtf(powf(cow2PlayerDis.y, 2.0f)));

	/*マイナスしているところは差が-値になっているから*/
	// 左右の量を計算
	if (cow2PlayerDis.x > 0 && naturalDis.y == 0) {
		// 牛から見て左にいるため牛飼いが右に動く
		directionValue[kCanMoveDirection::right] += naturalDis.x;

	} else if (cow2PlayerDis.x < 0 && naturalDis.y == 0) {
		// 牛から見て右にいるため牛飼いが左に動く
		directionValue[kCanMoveDirection::left] += naturalDis.x;
	}

	// 上下の量を計算
	if (cow2PlayerDis.y < 0 && naturalDis.x == 0) {
		// 牛から見て上にいるため牛飼いが下に動く
		directionValue[kCanMoveDirection::bottom] += naturalDis.y;

	} else if (cow2PlayerDis.y > 0 && naturalDis.x == 0) {
		// 牛から見て下にいるため牛飼いが上に動く
		directionValue[kCanMoveDirection::top] += naturalDis.y;
	}


	// 左上下の計算
	// xとyで値が小さい方を斜めの量として計算
	if (cow2PlayerDis.x > 0 && cow2PlayerDis.y < 0) {
		// 左上
		// 牛から見て左上にいるため牛飼いが右下に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にyの量を、残りは右に動く
			directionValue[kCanMoveDirection::rightBottom] += naturalDis.y;
			directionValue[kCanMoveDirection::right] += naturalDis.x;

		} else if (naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にxの量を、残りは下に動く
			directionValue[kCanMoveDirection::rightBottom] += naturalDis.x;
			directionValue[kCanMoveDirection::bottom] += naturalDis.y;

		} else {
			// 同じの場合はxとyを足して2で割った値斜めに足す
			directionValue[kCanMoveDirection::rightBottom] += ((naturalDis.x + naturalDis.y) / 2);
		}

	} else if (cow2PlayerDis.x > 0 && cow2PlayerDis.y > 0) {
		// 左下
		// 牛から見て左下にいるため牛飼いが右上に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にyの量を、残りは右に動く
			directionValue[kCanMoveDirection::rightTop] += naturalDis.y;
			directionValue[kCanMoveDirection::right] += naturalDis.x - naturalDis.y;

		} else if (naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にxの量を、残りは上に動く
			directionValue[kCanMoveDirection::rightTop] += naturalDis.x;
			directionValue[kCanMoveDirection::top] += naturalDis.y - naturalDis.x;

		} else {
			// 同じの場合はxとyを足して2で割った値斜めに足す
			directionValue[kCanMoveDirection::rightTop] += (naturalDis.x + naturalDis.y) / 2;
		}
	}

	// 右上下の計算
	if (cow2PlayerDis.x < 0 && cow2PlayerDis.y < 0) {
		// 右上
		// 牛から見て右上にいるため牛飼いが左下に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にy、残りは左に動く
			directionValue[kCanMoveDirection::leftBottom] += naturalDis.y;
			directionValue[kCanMoveDirection::left] += naturalDis.x - naturalDis.y;

		} else if (naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にx、残りは下に動く
			directionValue[kCanMoveDirection::leftBottom] += naturalDis.x;
			directionValue[kCanMoveDirection::bottom] += naturalDis.y - naturalDis.x;

		} else {
			// 同じの場合はxとyを足して2で割った値を斜めに足す
			directionValue[kCanMoveDirection::leftBottom] += (naturalDis.x + naturalDis.y) / 2;
		}

	} else if (cow2PlayerDis.x < 0 && cow2PlayerDis.y > 0) {
		// 右下
		// 牛から見て右下にいるため牛飼いが左上に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にy、残りは左に動く
			directionValue[kCanMoveDirection::leftTop] += naturalDis.y;
			directionValue[kCanMoveDirection::left] += naturalDis.x - naturalDis.y;

		} else if (naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にx、残りは上に動く
			directionValue[kCanMoveDirection::leftTop] += naturalDis.x;
			directionValue[kCanMoveDirection::top] += naturalDis.y - naturalDis.x;

		} else {
			// 同じの場合はxとyを足して2で割った値を斜めに足す
			directionValue[kCanMoveDirection::leftTop] += (naturalDis.x + naturalDis.y) / 2;
		}
	}

	for (int i = 0; i < 8; i++) {
		if (directionValue[i] != 0) {
			keepDire[i] = ((10 - std::clamp(directionValue[i], 0, 10)) * bullCow_->GetClampValue());
			/*keepDire[i] = ((10 - (directionValue[i])) * cow_->GetClampValue());*/

			bullCow_->SetGridDistanceValue(keepDire[i], i);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(i) + keepDire[i], i);
		}
	}
}

/*==========================================================================================================
												8方向全部を調べる
============================================================================================================*/
void BullCollision::CheckBullCowMoveAllDire() {

	// 壁までの距離を計算(端までを求めることでfor分の上限が分かる)
	int cow2topWall_ = mapChip_->GetMapChipRow() - 1 - bullCow_->GetCenterAdd().y;
	int cow2bottomWall_ = bullCow_->GetCenterAdd().y;
	int cow2leftWall_ = bullCow_->GetCenterAdd().x;
	int cow2rightWall_ = mapChip_->GetMapChipCol() - bullCow_->GetCenterAdd().x - 1;

	// topまでの計算
	for (int i = 1; i < cow2topWall_; i++) {
		// topまでの距離で牛飼いがいたら
		if (bullCow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x &&
			bullCow_->GetCenterAdd().y + i == cowherd_->GetCenterAdd().y) {
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAllDireValue() * 2,
				kCanMoveDirection::top);
		}

		// topまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (bullCow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				bullCow_->GetCenterAdd().y + i == youngPerson_->GetCenterAdd(j).y) {
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::top);
			}
		}
	}

	// bottomまでの計算
	for (int i = 1; i < cow2bottomWall_; i++) {
		// topまでの距離で牛飼いがいたら
		if (bullCow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x &&
			bullCow_->GetCenterAdd().y - i == cowherd_->GetCenterAdd().y) {
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAllDireValue() * 2,
				kCanMoveDirection::bottom);
		}

		// topまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (bullCow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				bullCow_->GetCenterAdd().y - i == youngPerson_->GetCenterAdd(j).y) {
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::bottom);
			}
		}
	}

	// leftまでの計算
	for (int i = 1; i < cow2leftWall_; i++) {
		// leftまでの距離で牛飼いがいたら
		if (bullCow_->GetCenterAdd().x - i == cowherd_->GetCenterAdd().x &&
			bullCow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAllDireValue() * 2,
				kCanMoveDirection::left);
		}

		// leftまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (bullCow_->GetCenterAdd().x - i == youngPerson_->GetCenterAdd(j).x &&
				bullCow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::left);
			}
		}
	}

	// rightまでの距離
	for (int i = 1; i < cow2rightWall_; i++) {
		// leftまでの距離で牛飼いがいたら
		if (bullCow_->GetCenterAdd().x + i == cowherd_->GetCenterAdd().x &&
			bullCow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAllDireValue() * 2,
				kCanMoveDirection::right);
		}

		// rightまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (bullCow_->GetCenterAdd().x + i == youngPerson_->GetCenterAdd(j).x &&
				bullCow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::right);
			}
		}
	}

	// leftTopの直線状
	int leftDeg = 1;
	int topDeg = 1;
	int bottomDeg = 1;
	int rightDeg = 1;

	if (cow2topWall_ > cow2leftWall_) {
		while (leftDeg != cow2leftWall_) {
			if (bullCow_->GetCenterAdd().x - leftDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y + topDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::leftTop);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x - leftDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y + topDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::leftTop);
					}
				}
			}
			leftDeg++;
			topDeg++;
		}
		leftDeg = 1;
		topDeg = 1;

	} else {
		while (topDeg != cow2topWall_) {
			if (bullCow_->GetCenterAdd().x + leftDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y + topDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::leftTop);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x + leftDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y + topDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::leftTop);
					}
				}
			}
			leftDeg++;
			topDeg++;
		}
		leftDeg = 1;
		topDeg = 1;
	}

	// 右上
	if (cow2topWall_ > cow2rightWall_) {
		while (rightDeg != cow2rightWall_) {
			if (bullCow_->GetCenterAdd().x + rightDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y + topDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::rightTop);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x + rightDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y + topDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::rightTop);
					}
				}
			}
			rightDeg++;
			topDeg++;
		}
		rightDeg = 1;
		topDeg = 1;

	} else {
		while (topDeg != cow2topWall_) {
			if (bullCow_->GetCenterAdd().x + rightDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y + topDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::rightTop);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x + rightDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y + topDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::rightTop);
					}
				}
			}
			rightDeg++;
			topDeg++;
		}
		rightDeg = 1;
		topDeg = 1;
	}

	// 左下
	if (bottomDeg > cow2leftWall_) {
		while (leftDeg != cow2leftWall_) {
			if (bullCow_->GetCenterAdd().x - leftDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y - bottomDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::leftBottom);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x - leftDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y - bottomDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::leftBottom);
					}
				}
			}
			leftDeg++;
			bottomDeg++;
		}
		leftDeg = 1;
		bottomDeg = 1;

	} else {
		while (bottomDeg != cow2bottomWall_) {
			if (bullCow_->GetCenterAdd().x - leftDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y - bottomDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::leftBottom);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x - leftDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y - bottomDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::leftBottom);
					}
				}
			}
			leftDeg++;
			bottomDeg++;
		}
		leftDeg = 1;
		bottomDeg = 1;
	}

	// 右下
	if (cow2bottomWall_ > cow2rightWall_) {
		while (rightDeg != cow2rightWall_) {
			if (bullCow_->GetCenterAdd().x + rightDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y - topDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::rightBottom);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x + rightDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y - topDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::rightBottom);
					}
				}
			}
			rightDeg++;
			bottomDeg++;
		}
		rightDeg = 1;
		bottomDeg = 1;

	} else {
		while (bottomDeg != cow2bottomWall_) {
			if (bullCow_->GetCenterAdd().x + rightDeg == cowherd_->GetCenterAdd().x &&
				bullCow_->GetCenterAdd().y - bottomDeg == cowherd_->GetCenterAdd().y) {

				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAllDireValue(),
					kCanMoveDirection::rightBottom);

				for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
					if (bullCow_->GetCenterAdd().x + rightDeg == youngPerson_->GetCenterAdd(j).x &&
						bullCow_->GetCenterAdd().y - bottomDeg == youngPerson_->GetCenterAdd(j).y) {
						bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAllDireValue(),
							kCanMoveDirection::rightBottom);
					}
				}
			}
			rightDeg++;
			bottomDeg++;
		}
		rightDeg = 1;
		bottomDeg = 1;
	}
}

/*==========================================================================================================
											雄牛から見て4つのエリアを見る
============================================================================================================*/
void BullCollision::CheckFourAreas() {
	// 4方向にいる人の数
	int personNum[4] = { 0 };
	int maxNumIndex = -1;
	int maxNum = 0;

	/* ------ cowherd ------ */
	// top
	if (cowherd_->GetCenterAdd().y > bullCow_->GetCenterAdd().y) {
		personNum[0]++;
	}

	// bottom
	if (cowherd_->GetCenterAdd().y < bullCow_->GetCenterAdd().y) {
		personNum[1]++;
	}

	// left
	if (cowherd_->GetCenterAdd().x < bullCow_->GetCenterAdd().x) {
		personNum[2]++;
	}

	// right
	if (cowherd_->GetCenterAdd().x > bullCow_->GetCenterAdd().x) {
		personNum[3]++;
	}

	/* ------ youngPerson ------ */
	for (int num = 0; num < youngPerson_->GetYoungMaxIndex(); num++) {
		// top
		if (youngPerson_->GetCenterAdd(num).y > bullCow_->GetCenterAdd().y) {
			personNum[0]++;
		}

		// bottom
		if (youngPerson_->GetCenterAdd(num).y < bullCow_->GetCenterAdd().y) {
			personNum[1]++;
		}

		// left
		if (youngPerson_->GetCenterAdd(num).x < bullCow_->GetCenterAdd().x) {
			personNum[2]++;
		}

		// right
		if (youngPerson_->GetCenterAdd(num).x > bullCow_->GetCenterAdd().x) {
			personNum[3]++;
		}
	}

	// 一番人数が多いエリアを調べる
	for (int dire = 0; dire < 4; dire++) {
		if (maxNum < personNum[dire]) {
			maxNum = personNum[dire];
			maxNumIndex = dire;
		}
	}

	// 4つのエリアで人がいる場合そのエリアの方向を減らす
	for (int dire = 0; dire < 4; dire++) {
		if (personNum[dire] != 0) {
			switch (dire) {
			case kCanMoveDirection::top:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::top);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::bottom);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::left);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::right);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);

				break;
			}

		} else {
			// 人がいない場合その方向への評価を加点する
			switch (dire) {
			case kCanMoveDirection::top:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetFourAreaValue(), kCanMoveDirection::top);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetFourAreaValue(), kCanMoveDirection::bottom);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetFourAreaValue(), kCanMoveDirection::left);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetFourAreaValue(), kCanMoveDirection::right);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);

				break;
			}
		}
	}
}

/*==========================================================================================================
									隣あっているものがないか確認
============================================================================================================*/
void BullCollision::CheckBullCowAdjoin() {
	// 牛と隣接しているものがないか調べる
	// top
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y + 1, bullCow_->GetCenterAdd().x) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::top);

	} else if (bullCow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y && bullCow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x) {
		// 上にいる場合は下に行って欲しいため下方向を多く加算する
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

		// また上に行かないように上に上方向を減算しておく
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetRockValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
	}

	// bottom
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y - 1, bullCow_->GetCenterAdd().x) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::bottom);

	} else if (bullCow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y && bullCow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x) {
		// 上に行ってほしい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);

		// 下に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetRockValue(), kCanMoveDirection::bottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);

	}

	// left
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y, bullCow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::left);

	} else if (bullCow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x && bullCow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
		// 右に行って欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

		// 左に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetRockValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
	}

	// right
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y, bullCow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::right);

	} else if (bullCow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x && bullCow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
		// 左にいって欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);

		// 右に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetRockValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);

	}

	// leftTop
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y + 1, bullCow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::leftTop);

	} else if (bullCow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y &&
		bullCow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x) {

		//　右下にいって欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

		// 左上に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAdjoinValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAdjoinValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetRockValue(), kCanMoveDirection::leftTop);

	}

	// rightTop
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y + 1, bullCow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::rightTop);

	} else if (bullCow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y &&
		bullCow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x) {

		// 左下に行って欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);

		// 右上に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAdjoinValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAdjoinValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetRockValue(), kCanMoveDirection::rightTop);

	}

	// leftBottom
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y - 1, bullCow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::leftBottom);

	} else if (bullCow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y &&
		bullCow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x) {

		// 右上に行って欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);

		// 左下に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetRockValue(), kCanMoveDirection::leftBottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAdjoinValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::bottom);

	}

	// rightBottom
	if (mapChip_->GetMapAdd(bullCow_->GetCenterAdd().y - 1, bullCow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		bullCow_->SetMoveDireValue(-bullCow_->GetRockValue(), kCanMoveDirection::rightBottom);

	} else if (bullCow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y &&
		bullCow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x) {

		// 左上に行って欲しい
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);

		// 右下に行かないで欲しい
		// 人がいる時はその方向を岩の値で減らす
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAdjoinValue(), kCanMoveDirection::right);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::bottom);
		bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetRockValue(), kCanMoveDirection::rightBottom);

	}

	//　ここから下は若人との判定を取る(for分があるため別に書く)
	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		// top
		if (bullCow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(i).x) {

			// 上にいる場合は下に行って欲しいため下方向を多く加算する
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

			// また上に行かないように上に上方向を減算しておく
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetRockValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightTop);


		}

		// bottom
		if (bullCow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(i).x) {

			// 上に行ってほしい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);

			// 下に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetRockValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
		}

		// left
		if (bullCow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x &&
			bullCow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(i).y) {

			// 右に行って欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

			// 左に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetRockValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
		}

		// right
		if (bullCow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x &&
			bullCow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(i).y) {

			// 左にいって欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);

			// 右に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetRockValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
		}

		// leftTop
		if (bullCow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x) {

			//　右下にいって欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightBottom);

			// 左上に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAdjoinValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAdjoinValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetRockValue(), kCanMoveDirection::leftTop);
		}

		// rightTop
		if (bullCow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x) {

			// 左下に行って欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftBottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::bottom);

			// 右上に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAdjoinValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAdjoinValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetRockValue(), kCanMoveDirection::rightTop);
		}

		// leftBottom
		if (bullCow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x) {
			// 右上に行って欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::rightTop);

			// 左下に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAdjoinValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetRockValue(), kCanMoveDirection::leftBottom);
		}

		// rightBottom
		if (bullCow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			bullCow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x) {

			// 左上に行って欲しい
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::top);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::left);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) + bullCow_->GetAdjoinAddValue(), kCanMoveDirection::leftTop);

			// 右下に行かないで欲しい
			// 人がいる時はその方向を岩の値で減らす
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAdjoinValue(), kCanMoveDirection::right);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::bottom);
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetRockValue(), kCanMoveDirection::rightBottom);
		}

	}

}

/*==========================================================================================================
									周りの計算
============================================================================================================*/
void BullCollision::CheckGridDire(const Vec2& add) {
	for (int dire = 0; dire < 8; dire++) {
		switch (dire) {
		case kCanMoveDirection::top:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - bullCow_->GetAdjoinValue(), kCanMoveDirection::top);
				}
			}
			break;

		case kCanMoveDirection::bottom:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::bottom);
				}
			}
			break;

		case kCanMoveDirection::left:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - bullCow_->GetAdjoinValue(), kCanMoveDirection::left);
				}
			}
			break;

		case kCanMoveDirection::right:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - bullCow_->GetAdjoinValue(), kCanMoveDirection::right);
				}
			}
			break;

		case kCanMoveDirection::leftTop:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
				}
			}
			break;

		case kCanMoveDirection::rightTop:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightTop) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
				}
			}

			break;

		case kCanMoveDirection::leftBottom:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
				}
			}
			break;

		case kCanMoveDirection::rightBottom:
			for (int i = 0; i < bullCow_->GetDireAddressNum(dire); i++) {
				if (bullCow_->GetCantMoveAdd(dire, i).x == add.x && bullCow_->GetCantMoveAdd(dire, i).y == add.y) {
					bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - bullCow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
				}
			}
			break;
		}
	}
}

/*==========================================================================================================
									同値があるか調べる
============================================================================================================*/

void BullCollision::CheckSameDireValue() {
	for (int i = 0; i < 7; i++) {
		for (int j = i + 1; j < 8; j++) {
			if (bullCow_->GetMoveDireValue(i) == bullCow_->GetMoveDireValue(j)) {
				if (bullCow_->GetMoveDireValue(i) != 100) {
					SameDireValue();
				}
			}
		}
	}
}

/*==========================================================================================================
									同値があった時の処理
============================================================================================================*/
void BullCollision::SameDireValue() {
	// 牧師との距離
	Vec2 cow2pDis;
	Vec2 naturalDis;

	cow2pDis.x = bullCow_->GetCenterAdd().x - cowherd_->GetCenterAdd().x;
	cow2pDis.y = bullCow_->GetCenterAdd().y - cowherd_->GetCenterAdd().y;

	naturalDis.x = static_cast<int>(sqrtf(powf((float)cow2pDis.x, 2.0f)));
	naturalDis.y = static_cast<int>(sqrtf(powf((float)cow2pDis.y, 2.0f)));

	// 距離が0だったら直線上にいると分かる
	if (naturalDis.x == 0) {
		// yの距離から上か下か調べる
		if (cow2pDis.y > 0) {
			// 下にいる
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			return;
		} else {
			// 上にいる
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			return;
		}
	}

	if (naturalDis.y == 0) {
		// xの距離から左か右か調べる
		if (cow2pDis.x > 0) {
			// 左にいる
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);
			return;
		} else {
			// 右にいる
			bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);
			return;
		}
	}

	// まずxの距離から右にいるか左にいるか調べる
	// xが0より大きいため左にいると分かる
	if (cow2pDis.x > 0) {
		// 次にyの距離から上にいるか下にいるか調べる
		// yが0より小さいため上にいると分かる
		if (cow2pDis.y < 0) {
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の左に行かないように左を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);

			} else {
				// 逆に上に行かないように上を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			}
		} else {
			// 下にいる
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の左に行かないように左を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);

			} else {
				// 逆に下に行かないように下を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			}
		}
	} else {
		// xが0より小さいため右にいると分かる
		// 次にyの距離から上にいるか下にいるか調べる
		// yが0より小さいため上にいると分かる
		if (cow2pDis.y < 0) {
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の右に行かないように右を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);

			} else {
				// 逆に上に行かないように上を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			}
		} else {
			// 下にいる
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の右に行かないように右を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);

			} else {
				// 逆に下に行かないように下を減算する
				bullCow_->SetMoveDireValue(bullCow_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			}
		}
	}
}

bool BullCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}