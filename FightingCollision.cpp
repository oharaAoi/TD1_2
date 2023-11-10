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
void FightingCollision::CheckCanMoveDire() {
	CheckRockAdjoin();

	/*--------------------------------------------------------------------------------------*/
	// 方向での評価
	CheckMoveDire(cowherd_->GetCenterAdd());

	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckMoveDire(youngPerson_->GetCenterAdd(i));
	}

	for (int row = 0; row < mapChip_->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip_->GetMapChipCol(); col++) {
			if (mapChip_->GetMapChipAdd()[row][col] == ChipType::ROCK) {
				Vec2 rockAdd = { col, row };
				CheckMoveDire(rockAdd);
			}
		}
	}
	/*--------------------------------------------------------------------------------------*/
}

/*========================================================
	押し戻し処理
========================================================*/


/*========================================================
	評価
========================================================*/


/*==========================================================================================================
										岩との評価
============================================================================================================*/
void FightingCollision::CheckRockAdjoin(){
	//　上
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y + 1][fighting_->GetWorldAdd().x] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - fighting_->GetRockValue(), kCanMoveDirection::top);
	}

	// 下
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y - 1][fighting_->GetWorldAdd().x] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - fighting_->GetRockValue(), kCanMoveDirection::bottom);
	}

	// 左
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y][fighting_->GetWorldAdd().x - 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - fighting_->GetRockValue(), kCanMoveDirection::left);
	}

	// 右
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y][fighting_->GetWorldAdd().x + 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - fighting_->GetRockValue(), kCanMoveDirection::right);
	}

	// 左上
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y + 1][fighting_->GetWorldAdd().x - 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftTop) - fighting_->GetRockValue(), kCanMoveDirection::leftTop);
	}

	// 右上
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y + 1][fighting_->GetWorldAdd().x + 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightTop) - fighting_->GetRockValue(), kCanMoveDirection::rightTop);
	}

	// 左下
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y - 1][fighting_->GetWorldAdd().x - 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftBottom) - fighting_->GetRockValue(), kCanMoveDirection::leftBottom);
	}

	// 右下
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y - 1][fighting_->GetWorldAdd().x + 1] == ChipType::ROCK) {
		fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightBottom) - fighting_->GetRockValue(), kCanMoveDirection::rightBottom);
	}
}

/*==========================================================================================================
										進行方向の評価(csvでやる)
============================================================================================================*/
void FightingCollision::CheckMoveDire(const Vec2& add){
	for (int dire = 0; dire < 8; dire++) {
		switch (dire) {
		case kCanMoveDirection::top:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - fighting_->GetAdjoinValue(), kCanMoveDirection::top);
				}
			}
			break;

		case kCanMoveDirection::bottom:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - fighting_->GetAdjoinValue(), kCanMoveDirection::bottom);
				}
			}
			break;

		case kCanMoveDirection::left:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - fighting_->GetAdjoinValue(), kCanMoveDirection::left);
				}
			}
			break;

		case kCanMoveDirection::right:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - fighting_->GetAdjoinValue(), kCanMoveDirection::right);
				}
			}
			break;
		}
	}
}

/*==========================================================================================================
										4つのエリアでの評価
============================================================================================================*/
void FightingCollision::CheckFourAreas(){
}