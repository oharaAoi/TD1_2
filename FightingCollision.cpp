#include "FightingCollision.h"

FightingCollision::FightingCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog) {
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
void FightingCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog) {
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
	CheckDogExist();

	CheckRockAdjoin();

	/*--------------------------------------------------------------------------------------*/
	for (int row = 0; row < mapChip_->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip_->GetMapChipCol(); col++) {
			if (mapChip_->GetMapChipAdd()[row][col] == ChipType::ROCK) {
				Vec2 rockAdd = { col, row };
				CheckMoveDireRock(rockAdd);
			}
		}
	}

	// 方向での評価
	/*CheckMoveDire(cowherd_->GetCenterAdd());*/

	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckMoveDire(youngPerson_->GetCenterAdd(i));
	}

	// 上の評価から実際に評価値を変える
	MoveDireDecision();
	/*--------------------------------------------------------------------------------------*/

	/*CheckFiveAreas(cowherd_->GetCenterAdd());*/

	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckFiveAreas(youngPerson_->GetCenterAdd(i));
	}

	FiveDireDecison();

	if (fighting_->GetAllPersonOnSlant() == true) {
		CheckFourAreas();
	}

	/*--------------------------------------------------------------------------------------*/
	CheckNearFence();

	/*--------------------------------------------------------------------------------------*/
	CheckSameDireValue();

}

void FightingCollision::CheckCollision() {
	RockCollision();

	FenceCollision();

	PersonCollision(cowherd_->GetCenterAdd());

	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		PersonCollision(youngPerson_->GetCenterAdd(i));
	}
}

/*========================================================
	押し戻し処理
========================================================*/

/*==========================================================================================================
										岩の押し戻し
============================================================================================================*/
void FightingCollision::RockCollision() {
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y][fighting_->GetWorldAdd().x] == ChipType::ROCK) {

		switch (fighting_->GetMovedDire()) {
		case kCanMoveDirection::top:
			fighting_->SetIsMove(false);
			fighting_->SetIsStan(true);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::bottom:
			fighting_->SetIsMove(false);
			fighting_->SetIsStan(true);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::left:
			fighting_->SetIsMove(false);
			fighting_->SetIsStan(true);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;

		case kCanMoveDirection::right:
			fighting_->SetIsMove(false);
			fighting_->SetIsStan(true);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) - mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;
		}
	}
}

/*==========================================================================================================
										柵の押し戻し
============================================================================================================*/

void FightingCollision::FenceCollision() {
	if (mapChip_->GetMapChipAdd()[fighting_->GetWorldAdd().y][fighting_->GetWorldAdd().x] == ChipType::FENCE) {

		switch (fighting_->GetMovedDire()) {
		case kCanMoveDirection::top:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::bottom:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::left:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;

		case kCanMoveDirection::right:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) - mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;
		}
	}
}

/*==========================================================================================================
										人の押し戻し
============================================================================================================*/

void FightingCollision::PersonCollision(const Vec2& add) {
	if (IsEqualAdd(fighting_->GetWorldAdd(), add)){

		switch (fighting_->GetMovedDire()) {
		case kCanMoveDirection::top:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				(fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y) + (mapChip_->GetTileSize().y * 0.5f) - mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::bottom:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f) + mapChip_->GetTileSize().y,
				}
			);
			break;

		case kCanMoveDirection::left:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) + mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;

		case kCanMoveDirection::right:
			fighting_->SetIsMove(false);

			fighting_->SetWorldPos({
				(fighting_->GetWorldAdd().x * mapChip_->GetTileSize().x) - mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
				fighting_->GetWorldAdd().y * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f),
				}
			);
			break;
		}
	}
}

/*========================================================
	評価
========================================================*/
/*==========================================================================================================
										犬との評価
============================================================================================================*/
void FightingCollision::CheckDogExist() {
	// 犬がいる方向を入手しいる場合は反対方向に値を増やす
	switch (dog_->GetIsExistSide()) {
	case ExsitSide::TOP:
		fighting_->SetMoveDireValue(fighting_->GetDogValue(), kCanMoveDirection::bottom);
		break;

	case ExsitSide::BOTTOM:
		fighting_->SetMoveDireValue(fighting_->GetDogValue(), kCanMoveDirection::top);
		break;

	case ExsitSide::LEFT:
		fighting_->SetMoveDireValue(fighting_->GetDogValue(), kCanMoveDirection::right);
		break;

	case ExsitSide::RIGHT:
		fighting_->SetMoveDireValue(fighting_->GetDogValue(), kCanMoveDirection::left);
		break;
	}
}

/*==========================================================================================================
										岩との評価
============================================================================================================*/
void FightingCollision::CheckRockAdjoin() {
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

void FightingCollision::MoveDireDecision() {

	int nearPerson[4];
	int preMostIndex = 0;
	int mostNearPersonDire = 99;

	bool isMostPerson[4] = { 0 };

	for (int dire = 0; dire < 4; dire++) {
		for (int index = 0; index < 3; index++) {
			// 人がいる配列の1個、2個前を見て岩があったら加算しない
			if (fighting_->GetIsMoveDireBlock(dire, 0) == true) {
				// 牛から見て一番近い置に位ブロックがあったらbreak
				break;

			} else if (fighting_->GetIsMoveDirePreson(dire, index) == true) {
				// 遠い位置から見てそこに人がいたら
				nearPerson[dire] = index;
				break;
			}
		}
	}

	// 位置値が低い人を探す
	for (int dire = 0; dire < 4; dire++) {
		for (int index = 0; index < 3; index++) {
			if (fighting_->GetIsMoveDirePreson(dire, index) == true) {
				if (mostNearPersonDire > nearPerson[dire]) {

					// 一番近い人の方向
					mostNearPersonDire = nearPerson[dire];

					preMostIndex = nearPerson[dire];
					// 一番近い人であるかどうか
					isMostPerson[dire] = true;

				} else if (mostNearPersonDire == nearPerson[dire]) {
					//　同値がいる場合はどっちも加算するためにフラグをtrueにする
					isMostPerson[dire] = true;
				}
			}
		}
	}

	// 実際に値を入れる
	for (int dire = 0; dire < 4; dire++) {
		if (isMostPerson[dire]) {
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::top) + fighting_->GetAllDireValue(),
					kCanMoveDirection::top
				);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::bottom) + fighting_->GetAllDireValue(),
					kCanMoveDirection::bottom
				);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::left) + fighting_->GetAllDireValue(),
					kCanMoveDirection::left
				);
				break;

			case kCanMoveDirection::right:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::right) + fighting_->GetAllDireValue(),
					kCanMoveDirection::right
				);
				break;
			}
		}
	}
}

/*==========================================================================================================
										進行方向の評価(csvでやる)
============================================================================================================*/
void FightingCollision::CheckMoveDire(const Vec2& add) {
	for (int dire = 0; dire < 4; dire++) {
		switch (dire) {
		case kCanMoveDirection::top:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDirePreson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::bottom:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDirePreson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::left:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDirePreson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::right:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDirePreson(dire, i, true);
				}
			}
			break;
		}
	}
}

/*==========================================================================================================
										進行方向の評価(岩がある場合)
============================================================================================================*/
void FightingCollision::CheckMoveDireRock(const Vec2& add) {
	for (int dire = 0; dire < 8; dire++) {
		switch (dire) {
		case kCanMoveDirection::top:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDireBlock(kCanMoveDirection::top, i, true);
				}
			}
			break;

		case kCanMoveDirection::bottom:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDireBlock(kCanMoveDirection::bottom, i, true);
				}
			}
			break;

		case kCanMoveDirection::left:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDireBlock(kCanMoveDirection::left, i, true);
				}
			}
			break;

		case kCanMoveDirection::right:
			for (int i = 0; i < fighting_->GetDireAddressNum(dire); i++) {
				if (fighting_->GetCantMoveAdd(dire, i).x == add.x && fighting_->GetCantMoveAdd(dire, i).y == add.y) {
					fighting_->SetIsMoveDireBlock(kCanMoveDirection::right, i, true);
				}
			}
			break;
		}
	}
}

/*==========================================================================================================
										5つのエリアの評価
============================================================================================================*/
void FightingCollision::CheckFiveAreas(const Vec2& add) {
	// 人が5つの範囲でどこにいるか調べる
	for (int dire = 0; dire < 5; dire++) {
		switch (dire) {
		case kCanMoveDirection::top:
			for (int i = 0; i < fighting_->GetAllDireGridAddress(dire); i++) {
				if (fighting_->GetAllDireGridAddress(dire, i).x == add.x && fighting_->GetAllDireGridAddress(dire, i).y == add.y) {
					fighting_->SetIsAllDireOnPerson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::bottom:
			for (int i = 0; i < fighting_->GetAllDireGridAddress(dire); i++) {
				if (fighting_->GetAllDireGridAddress(dire, i).x == add.x && fighting_->GetAllDireGridAddress(dire, i).y == add.y) {
					fighting_->SetIsAllDireOnPerson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::left:
			for (int i = 0; i < fighting_->GetAllDireGridAddress(dire); i++) {
				if (fighting_->GetAllDireGridAddress(dire, i).x == add.x && fighting_->GetAllDireGridAddress(dire, i).y == add.y) {
					fighting_->SetIsAllDireOnPerson(dire, i, true);
				}
			}
			break;

		case kCanMoveDirection::right:
			for (int i = 0; i < fighting_->GetAllDireGridAddress(dire); i++) {
				if (fighting_->GetAllDireGridAddress(dire, i).x == add.x && fighting_->GetAllDireGridAddress(dire, i).y == add.y) {
					fighting_->SetIsAllDireOnPerson(dire, i, true);
				}
			}
			break;

		case 4:
			for (int i = 0; i < fighting_->GetAllDireGridAddress(dire); i++) {
				if (fighting_->GetAllDireGridAddress(dire, i).x == add.x && fighting_->GetAllDireGridAddress(dire, i).y == add.y) {
					fighting_->SetIsAllDireOnPerson(dire, i, true);
				}
			}
			break;
		}
	}
}

/*==========================================================================================================
										5つのエリアでの最終評価
============================================================================================================*/

void FightingCollision::FiveDireDecison() {

	int mostNearPersonIndex = 999;

	bool isMostPerson[5] = { 0 };

	// 一番近い人を探す
	for (int dire = 0; dire < 5; dire++) {
		for (int index = 0; index < fighting_->GetAllDireGridAddress(dire); index++) {
			if (fighting_->GetIsOnPreson(dire, index) == true) {
				if (mostNearPersonIndex > index) {

					// 一番近い人の方向
					mostNearPersonIndex = index;

					for (int i = 0; i < 5; i++) {
						isMostPerson[i] = false;
					}

					// 一番近い人であるかどうか
					isMostPerson[dire] = true;

					if (dire == 4) {
						fighting_->SetAllPersonOnSlant(true);
					}

				} else if (mostNearPersonIndex == index) {
					//　同値がいる場合はどっちも加算するためにフラグをtrueにする
					isMostPerson[dire] = true;
				}
			}
		}
	}

	// 斜めが一番近かった時
	if (fighting_->GetAllPersonOnSlant() == true) {
		return;
	}

	for (int dire = 0; dire < 5; dire++) {
		if (isMostPerson[dire]) {
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::top) + fighting_->GetAllDireValue(),
					kCanMoveDirection::top
				);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::bottom) + fighting_->GetAllDireValue(),
					kCanMoveDirection::bottom
				);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::left) + fighting_->GetAllDireValue(),
					kCanMoveDirection::left
				);
				break;

			case kCanMoveDirection::right:
				fighting_->SetMoveDireValue(
					fighting_->GetMoveDireValue(kCanMoveDirection::right) + fighting_->GetAllDireValue(),
					kCanMoveDirection::right
				);
				break;
			}
		}
	}

}

/*==========================================================================================================
										4つのエリアでの評価
============================================================================================================*/
void FightingCollision::CheckFourAreas() {

	// 4方向にいる人の数
	int personNum[4] = { 0 };
	int maxNumIndex = -1;
	int maxNum = 0;

	bool maxOnPresonIndex[4] = { false };

	/* ------ cowherd ------ */
	// top
	if (cowherd_->GetCenterAdd().y > fighting_->GetWorldAdd().y) {
		personNum[0]++;
	}

	// bottom
	if (cowherd_->GetCenterAdd().y < fighting_->GetWorldAdd().y) {
		personNum[1]++;
	}

	// left
	if (cowherd_->GetCenterAdd().x < fighting_->GetWorldAdd().x) {
		personNum[2]++;
	}

	// right
	if (cowherd_->GetCenterAdd().x > fighting_->GetWorldAdd().x) {
		personNum[3]++;
	}

	/* ------ youngPerson ------ */
	for (int num = 0; num < youngPerson_->GetYoungMaxIndex(); num++) {
		// top
		if (youngPerson_->GetCenterAdd(num).y > fighting_->GetWorldAdd().y) {
			personNum[0]++;
		}

		// bottom
		if (youngPerson_->GetCenterAdd(num).y < fighting_->GetWorldAdd().y) {
			personNum[1]++;
		}

		// left
		if (youngPerson_->GetCenterAdd(num).x < fighting_->GetWorldAdd().x) {
			personNum[2]++;
		}

		// right
		if (youngPerson_->GetCenterAdd(num).x > fighting_->GetWorldAdd().x) {
			personNum[3]++;
		}
	}

	// 一番人数が多いエリアを調べる
	for (int dire = 0; dire < 4; dire++) {
		if (maxNum < personNum[dire]) {
			maxNum = personNum[dire];
			maxNumIndex = dire;

			// 一旦すべてfalseにする
			for (int i = 0; i < 4; i++) {
				maxOnPresonIndex[i] = false;
			}

			maxOnPresonIndex[dire] = true;

		} else if (maxNum == personNum[dire]) {
			maxOnPresonIndex[dire] = true;
		}
	}

	// 4つのエリアで人がいる場合そのエリアの方向を増やす
	for (int dire = 0; dire < 4; dire++) {
		if (maxOnPresonIndex[dire]) {
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) + fighting_->GetFourAreaValue(), kCanMoveDirection::top);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::bottom);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) + fighting_->GetFourAreaValue(), kCanMoveDirection::left);
				break;

			case  kCanMoveDirection::right:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) + fighting_->GetFourAreaValue(), kCanMoveDirection::right);
				break;
			}
		}
	}
}

/*==========================================================================================================
										柵との評価
============================================================================================================*/

void FightingCollision::CheckNearFence() {
	int nearWallOfValue[4]{};

	nearWallOfValue[kCanMoveDirection::top] = mapChip_->GetMapChipRow() - fighting_->GetWorldAdd().y - 1;
	nearWallOfValue[kCanMoveDirection::bottom] = fighting_->GetWorldAdd().y;

	nearWallOfValue[kCanMoveDirection::left] = fighting_->GetWorldAdd().x;
	nearWallOfValue[kCanMoveDirection::right] = mapChip_->GetMapChipCol() - fighting_->GetWorldAdd().x - 1;

	int preMostIndex = 0;
	int mostNearWallDire = 99;

	bool isMostNearWall[4] = { 0 };

	// 最小値を求める
	for (int i = 0; i < 4; i++) {
		if (mostNearWallDire > nearWallOfValue[i]) {

			isMostNearWall[preMostIndex] = false;
			mostNearWallDire = nearWallOfValue[i];
			isMostNearWall[i] = true;
			preMostIndex = i;

		} else if (mostNearWallDire == nearWallOfValue[i]) {
			isMostNearWall[i] = true;
		}
	}

	// 評価をする
	for (int dire = 0; dire < 4; dire++) {
		if (isMostNearWall[dire]) {
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) + fighting_->GetFenceValue(), kCanMoveDirection::top);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) + fighting_->GetFenceValue(), kCanMoveDirection::bottom);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) + fighting_->GetFenceValue(), kCanMoveDirection::left);
				break;

			case  kCanMoveDirection::right:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) + fighting_->GetFenceValue(), kCanMoveDirection::right);
				break;
			}
		}
	}
}

void FightingCollision::CheckSameDireValue() {
	for (int i = 0; i < 3; i++) {
		for (int j = i + 1; j < 4; j++) {
			if (fighting_->GetMoveDireValue(i) == fighting_->GetMoveDireValue(j)) {
				SameDireValue();
			}
		}
	}
}

void FightingCollision::SameDireValue() {
	// 牧師との距離
	Vec2 cow2pDis;
	Vec2 naturalDis;

	cow2pDis.x = fighting_->GetWorldAdd().x - cowherd_->GetCenterAdd().x;
	cow2pDis.y = fighting_->GetWorldAdd().y - cowherd_->GetCenterAdd().y;

	naturalDis.x = static_cast<int>(sqrtf(powf((float)cow2pDis.x, 2.0f)));
	naturalDis.y = static_cast<int>(sqrtf(powf((float)cow2pDis.y, 2.0f)));

	// 距離が0だったら直線上にいると分かる
	if (naturalDis.x == 0) {
		// yの距離から上か下か調べる
		if (cow2pDis.y > 0) {
			// 下にいる
			fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			return;
		} else {
			// 上にいる
			fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			return;
		}
	}

	if (naturalDis.y == 0) {
		// xの距離から左か右か調べる
		if (cow2pDis.x > 0) {
			// 左にいる
			fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);
			return;
		} else {
			// 右にいる
			fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);
			return;
		}
	}

	// まずxの距離から右にいるか左にいるか調べる
	// xが0より大きいため左にいると分かる
	if (cow2pDis.x > 0) {
		// 次にyの距離から上にいるか下にいるか調べる
		// yが0より大きいため上にいると分かる
		if (cow2pDis.y > 0) {
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の左に行かないように左を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);

			} else {
				// 逆に上に行かないように上を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			}
		} else {
			// 下にいる
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の左に行かないように左を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - 15, kCanMoveDirection::left);

			} else {
				// 逆に下に行かないように下を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			}
		}
	} else {
		// xが0より小さいため右にいると分かる
		// 次にyの距離から上にいるか下にいるか調べる
		// yが0より大きいため上にいると分かる
		if (cow2pDis.y > 0) {
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の右に行かないように右を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);

			} else {
				// 逆に上に行かないように上を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - 15, kCanMoveDirection::top);
			}
		} else {
			// 下にいる
			// xとyの距離を比較してxの方が大きい場合x方向に進むと牧師の前に来てしまう可能性がある
			if (naturalDis.x > naturalDis.y) {
				// そのためx方向の右に行かないように右を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - 15, kCanMoveDirection::right);

			} else {
				// 逆に下に行かないように下を減算する
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - 15, kCanMoveDirection::bottom);
			}
		}
	}

}

bool FightingCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}