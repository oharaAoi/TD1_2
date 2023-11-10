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

	int nearPerson[4] = { 0 };
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
	for (int dire = 0; dire < 3; dire++) {
		if (nearPerson[dire] != 0) {
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

	for (int dire = 0; dire < 8; dire++) {
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
		}
	}

	// 4つのエリアで人がいる場合そのエリアの方向を減らす
	for (int dire = 0; dire < 4; dire++) {
		if (personNum[dire] != 0) {
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::top);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftTop) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightTop) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::bottom);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftBottom) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightBottom) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::left);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftTop) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftBottom) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::right);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightTop) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightBottom) - fighting_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);

				break;
			}

		} else {
			// 人がいない場合その方向への評価を加点する
			switch (dire) {
			case kCanMoveDirection::top:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::top) + fighting_->GetFourAreaValue(), kCanMoveDirection::top);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftTop) + fighting_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightTop) + fighting_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::bottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::bottom);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftBottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightBottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::left) + fighting_->GetFourAreaValue(), kCanMoveDirection::left);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftTop) + fighting_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::leftBottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::right) + fighting_->GetFourAreaValue(), kCanMoveDirection::right);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightTop) + fighting_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				fighting_->SetMoveDireValue(fighting_->GetMoveDireValue(kCanMoveDirection::rightBottom) + fighting_->GetFourAreaValue(), kCanMoveDirection::rightBottom);

				break;
			}
		}
	}


}