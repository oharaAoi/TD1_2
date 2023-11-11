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
	CheckMoveDire(cowherd_->GetCenterAdd());

	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckMoveDire(youngPerson_->GetCenterAdd(i));
	}

	// 上の評価から実際に評価値を変える
	MoveDireDecision();
	/*--------------------------------------------------------------------------------------*/

	CheckFourAreas();


	/*--------------------------------------------------------------------------------------*/
	CheckNearFence();

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

	int nearPerson[4] = { 100 };
	int preMostIndex = 0;
	int mostNearPersonDire = 99;

	bool isMostPerson[4] = { 0 };

	for (int dire = 0; dire < 4; dire++) {
		for (int index = 0; index < 3; index++) {
			// 人がいる配列の1個、2個前を見て岩があったら加算しない
			if (fighting_->GetIsMoveDireBlock(dire, 0) == true) {
				// 牛から見て一番近い位置にブロックがあったらbreak
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

bool FightingCollision::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}