#include "CowCollision.h"

CowCollision::CowCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog) {
	Init(cowherd, youngPerson, mapChip, cow, dog);
}

CowCollision::~CowCollision() {
	Finalize();
}

void CowCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
	dog_ = dog;
}

void CowCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

void CowCollision::CheckCowMoveDire() {
	cow_->DireInit();

	// 犬がいる時
	CheckDogExist();

	// 両隣フェンスの時
	CheckCowAdjoin();
	/*CheckCowFourArea();*/
	// マス目の計算(牧師
	CheckGridDistance(cowherd_->GetCenterAdd());

	// マス目の計算(若人
	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		CheckGridDistance(youngPerson_->GetCenterAdd(i));
	}

	// 4つのエリア
	CheckFourAreas();

	// 牛から見た全方向
	CheckCowMoveAllDire();

	// 牛がフェンスと隣の時
	CheckFenseCollision();
}

/*=================================================================
	牛が移動してフェンス外に行ってしまったら
=================================================================*/

void CowCollision::CheckFenseScissorsCollision() {
	// 両隣外かフェンスだったら
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x + 1] == ChipType::FENCE &&
		mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x - 1] == ChipType::FENCE) {

		// 真ん中により上だったら下に戻す
		if (cow_->GetCenterAdd().y > mapChip_->GetMapChipRow() / 2.0f) {
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x, cow_->GetWorldCneterPos().y - mapChip_->GetTileSize().y });

			// 真ん中により下だったら上に戻す
		} else if (cow_->GetCenterAdd().y < mapChip_->GetMapChipRow() / 2.0f) {
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x, cow_->GetWorldCneterPos().y + mapChip_->GetTileSize().x });
		}
	}

	// 上下がフェンスだったら
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y + 1][cow_->GetCenterAdd().x] == ChipType::FENCE &&
		mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y - 1][cow_->GetCenterAdd().x] == ChipType::FENCE) {

		// 真ん中により右だったら左に戻す
		if (cow_->GetCenterAdd().x > mapChip_->GetMapChipCol() / 2.0f) {
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x - mapChip_->GetTileSize().x , cow_->GetWorldCneterPos().y });

			// 真ん中により左だったら右に戻す
		} else if (cow_->GetCenterAdd().x < mapChip_->GetMapChipCol() / 2.0f) {
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x + mapChip_->GetTileSize().x, cow_->GetWorldCneterPos().y });
		}

	}
}

/*=================================================================
	牛が移動して岩に行ってしまったら
=================================================================*/
void CowCollision::CheckRockCollision() {
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x] == ChipType::ROCK) {

		switch (cow_->GetMovedDire()) {
		case kCanMoveDirection::top:
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x, cow_->GetWorldCneterPos().y - mapChip_->GetTileSize().y });
			break;

		case kCanMoveDirection::bottom:
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x, cow_->GetWorldCneterPos().y + mapChip_->GetTileSize().y });
			break;

		case kCanMoveDirection::left:
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x + mapChip_->GetTileSize().x, cow_->GetWorldCneterPos().y });
			break;

		case kCanMoveDirection::right:
			cow_->SetWorldCenterPos({ cow_->GetWorldCneterPos().x - mapChip_->GetTileSize().x, cow_->GetWorldCneterPos().y });
			break;

		case kCanMoveDirection::leftTop:
			cow_->SetWorldCenterPos({
				cow_->GetWorldCneterPos().x + mapChip_->GetTileSize().x,
				cow_->GetWorldCneterPos().y - mapChip_->GetTileSize().y
				});
			break;

		case kCanMoveDirection::rightTop:
			cow_->SetWorldCenterPos({
				cow_->GetWorldCneterPos().x - mapChip_->GetTileSize().x,
				cow_->GetWorldCneterPos().y - mapChip_->GetTileSize().y
				});
			break;

		case kCanMoveDirection::leftBottom:
			cow_->SetWorldCenterPos({
				cow_->GetWorldCneterPos().x + mapChip_->GetTileSize().x,
				cow_->GetWorldCneterPos().y + mapChip_->GetTileSize().y
				});
			break;

		case kCanMoveDirection::rightBottom:
			cow_->SetWorldCenterPos({
				cow_->GetWorldCneterPos().x - mapChip_->GetTileSize().x,
				cow_->GetWorldCneterPos().y + mapChip_->GetTileSize().y
				});
			break;
		}
	}
}


/*=================================================================
	方向の評価
=================================================================*/
void CowCollision::CheckDogExist() {
	// 犬がいる方向を入手しいる場合は値を減らす
	switch (dog_->GetIsExistSide()) {
	case ExsitSide::TOP:
		cow_->SetMoveDireValue(cow_->GetDogValue(), kCanMoveDirection::bottom);
		break;

	case ExsitSide::BOTTOM:
		cow_->SetMoveDireValue(cow_->GetDogValue(), kCanMoveDirection::top);
		break;

	case ExsitSide::LEFT:
		cow_->SetMoveDireValue(cow_->GetDogValue(), kCanMoveDirection::right);
		break;

	case ExsitSide::RIGHT:
		cow_->SetMoveDireValue(cow_->GetDogValue(), kCanMoveDirection::left);
		break;
	}
}

/*=================================================================
	牛が移動してフェンスと隣あったら
=================================================================*/
void CowCollision::CheckFenseCollision() {
	// top
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y + 1][cow_->GetCenterAdd().x] == ChipType::FENCE) {
		cow_->SetMoveDireValue(cow_->GetFenceValue(), kCanMoveDirection::top);
	}

	// bottom
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y - 1][cow_->GetCenterAdd().x] == ChipType::FENCE) {
		cow_->SetMoveDireValue(cow_->GetFenceValue(), kCanMoveDirection::bottom);
	}

	// left
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x - 1] == ChipType::FENCE) {
		cow_->SetMoveDireValue(cow_->GetFenceValue(), kCanMoveDirection::left);
	}

	// right
	if (mapChip_->GetMapChipAdd()[cow_->GetCenterAdd().y][cow_->GetCenterAdd().x + 1] == ChipType::FENCE) {
		cow_->SetMoveDireValue(cow_->GetFenceValue(), kCanMoveDirection::right);
	}
}


/*=================================================================
	隣接しているかどうか
=================================================================*/
void CowCollision::CheckCowAdjoin() {
	// 牛と隣接しているものがないか調べる

	// top
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::top);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::top);

	} else if (cow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y && cow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x) {
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
	}

	// bottom
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::bottom);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::bottom);

	} else if (cow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y && cow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x) {
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);

	}

	// left
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y, cow_->GetCenterAdd().x - 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::left);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y, cow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::left);

	} else if (cow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x && cow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
	}

	// right
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y, cow_->GetCenterAdd().x + 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::right);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y, cow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::right);

	} else if (cow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x && cow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);

	}

	// leftTop
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x - 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::leftTop);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::leftTop);

	} else if (cow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y &&
		cow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x) {

		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
	}

	// rightTop
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x + 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::rightTop);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y + 1, cow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::rightTop);

	} else if (cow_->GetCenterAdd().y + 1 == cowherd_->GetCenterAdd().y &&
		cow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x) {

		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);

	}

	// leftBottom
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x - 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::leftBottom);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x - 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::leftBottom);

	} else if (cow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y &&
		cow_->GetCenterAdd().x - 1 == cowherd_->GetCenterAdd().x) {

		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);

	}

	// rightBottom
	if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x + 1) == ChipType::FENCE) {
		cow_->SetMoveDireValue(-cow_->GetFenceValue(), kCanMoveDirection::rightBottom);

	} else if (mapChip_->GetMapAdd(cow_->GetCenterAdd().y - 1, cow_->GetCenterAdd().x + 1) == ChipType::ROCK) {
		cow_->SetMoveDireValue(-cow_->GetRockValue(), kCanMoveDirection::rightBottom);

	} else if (cow_->GetCenterAdd().y - 1 == cowherd_->GetCenterAdd().y &&
		cow_->GetCenterAdd().x + 1 == cowherd_->GetCenterAdd().x) {

		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
		cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);

	}

	//　ここから下は若人との判定を取る(for分があるため別に書く)
	for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
		// top
		if (cow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) +cow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
		}

		// bottom
		if (cow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
		}

		// left
		if (cow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x &&
			cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(i).y) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
		}

		// right
		if (cow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x &&
			cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(i).y) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::leftBottom);
		}

		// leftTop
		if (cow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetAdjoinValue(), kCanMoveDirection::rightBottom);
		}

		// rightTop
		if (cow_->GetCenterAdd().y + 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetAdjoinValue(), kCanMoveDirection::bottom);
		}

		// leftBottom
		if (cow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x - 1 == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetAdjoinValue(), kCanMoveDirection::right);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetAdjoinValue(), kCanMoveDirection::rightTop);
		}

		// rightBottom
		if (cow_->GetCenterAdd().y - 1 == youngPerson_->GetCenterAdd(i).y &&
			cow_->GetCenterAdd().x + 1 == youngPerson_->GetCenterAdd(i).x) {

			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetAdjoinValue(), kCanMoveDirection::top);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetAdjoinValue(), kCanMoveDirection::left);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetAdjoinValue(), kCanMoveDirection::leftTop);
		}

	}

}

/*=================================================================
	4つのエリアを調べる(この関数は使わなくなるかも)
=================================================================*/
void CowCollision::CheckCowFourArea() {
	// 左上エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis < mapChip_->GetMapChipRow(); yAxis++) {
		for (int xAxis = 0; xAxis < cow_->GetCenterAdd().x; xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetFourAreaValue(), kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - cow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetFourAreaValue(), kCanMoveDirection::top);
			}

			// 若人
			for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetFourAreaValue(), kCanMoveDirection::left);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - cow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetFourAreaValue(), kCanMoveDirection::top);
				}
			}
		}
	}

	// 右上エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis < mapChip_->GetMapChipRow(); yAxis++) {
		for (int xAxis = cow_->GetCenterAdd().x; xAxis < mapChip_->GetMapChipCol(); xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetFourAreaValue(), kCanMoveDirection::top);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) - cow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetFourAreaValue(), kCanMoveDirection::right);
			}

			// 若人
			for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetFourAreaValue(), kCanMoveDirection::top);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetFourAreaValue(), kCanMoveDirection::right);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) - cow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				}
			}
		}
	}

	// 左下エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis > 0; yAxis--) {
		for (int xAxis = 0; xAxis < cow_->GetCenterAdd().x; xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetFourAreaValue(), kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetFourAreaValue(), kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - cow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
			}

			// 若人
			for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetFourAreaValue(), kCanMoveDirection::bottom);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetFourAreaValue(), kCanMoveDirection::left);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - cow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				}
			}
		}
	}

	// 右下エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis > 0; yAxis--) {
		for (int xAxis = cow_->GetCenterAdd().x; xAxis < mapChip_->GetMapChipCol(); xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetFourAreaValue(), kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetFourAreaValue(), kCanMoveDirection::right);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - cow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);
			}

			// 若人
			for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetFourAreaValue(), kCanMoveDirection::bottom);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetFourAreaValue(), kCanMoveDirection::right);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - cow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);
				}
			}
		}
	}
}

/*=================================================================
	4つのエリアを調べる
=================================================================*/
void CowCollision::CheckFourAreas() {
	// 4方向にいる人の数
	int personNum[4] = { 0 };
	int maxNumIndex = -1;
	int maxNum = 0;

	/* ------ cowherd ------ */
	// top
	if (cowherd_->GetCenterAdd().y > cow_->GetCenterAdd().y) {
		personNum[0]++;
	}

	// bottom
	if (cowherd_->GetCenterAdd().y < cow_->GetCenterAdd().y) {
		personNum[1]++;
	}

	// left
	if (cowherd_->GetCenterAdd().x < cow_->GetCenterAdd().x) {
		personNum[2]++;
	}

	// right
	if (cowherd_->GetCenterAdd().x > cow_->GetCenterAdd().x) {
		personNum[3]++;
	}

	/* ------ youngPerson ------ */
	for (int num = 0; num < youngPerson_->GetYoungMaxIndex(); num++) {
		// top
		if (youngPerson_->GetCenterAdd(num).y > cow_->GetCenterAdd().y) {
			personNum[0]++;
		}

		// bottom
		if (youngPerson_->GetCenterAdd(num).y < cow_->GetCenterAdd().y) {
			personNum[1]++;
		}

		// left
		if (youngPerson_->GetCenterAdd(num).x < cow_->GetCenterAdd().x) {
			personNum[2]++;
		}

		// right
		if (youngPerson_->GetCenterAdd(num).x > cow_->GetCenterAdd().x) {
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
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::top);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::right);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - cow_->GetFourAreaValue() * personNum[dire], kCanMoveDirection::rightBottom);

				break;
			}

		} else {
			// 人がいない場合その方向への評価を加点する
			switch (dire) {
			case kCanMoveDirection::top:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) + cow_->GetFourAreaValue(), kCanMoveDirection::top);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				break;

			case kCanMoveDirection::bottom:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) + cow_->GetFourAreaValue(), kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + cow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);
				break;

			case kCanMoveDirection::left:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) + cow_->GetFourAreaValue(), kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) + cow_->GetFourAreaValue(), kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) + cow_->GetFourAreaValue(), kCanMoveDirection::leftBottom);
				break;

			case  kCanMoveDirection::right:
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) + cow_->GetFourAreaValue(), kCanMoveDirection::right);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) + cow_->GetFourAreaValue(), kCanMoveDirection::rightTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) + cow_->GetFourAreaValue(), kCanMoveDirection::rightBottom);

				break;
			}
		}
	}
}

/*=================================================================
	牛の8方向を調べる
=================================================================*/
void CowCollision::CheckCowMoveAllDire() {
	// 壁までの距離を計算(端までを求めることでfor分の上限が分かる)
	int cow2topWall_ = mapChip_->GetMapChipRow() - 1 - cow_->GetCenterAdd().y;
	int cow2bottomWall_ = cow_->GetCenterAdd().y;
	int cow2leftWall_ = cow_->GetCenterAdd().x;
	int cow2rightWall_ = mapChip_->GetMapChipCol() - cow_->GetCenterAdd().x - 1;

	// topまでの計算
	for (int i = 1; i < cow2topWall_; i++) {
		// topまでの距離で牛飼いがいたら
		if (cow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x &&
			cow_->GetCenterAdd().y + i == cowherd_->GetCenterAdd().y) {
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetAllDireValue(),
				kCanMoveDirection::top);
		}

		// topまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y + i == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetAllDireValue(),
					kCanMoveDirection::top);
			}
		}
	}

	// bottomまでの計算
	for (int i = 1; i < cow2bottomWall_; i++) {
		// topまでの距離で牛飼いがいたら
		if (cow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x &&
			cow_->GetCenterAdd().y - i == cowherd_->GetCenterAdd().y) {
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetAllDireValue(),
				kCanMoveDirection::bottom);
		}

		// topまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y - i == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetAllDireValue(),
					kCanMoveDirection::bottom);
			}
		}
	}

	// leftまでの計算
	for (int i = 1; i < cow2leftWall_; i++) {
		// leftまでの距離で牛飼いがいたら
		if (cow_->GetCenterAdd().x - i == cowherd_->GetCenterAdd().x &&
			cow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetAllDireValue(),
				kCanMoveDirection::left);
		}

		// leftまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (cow_->GetCenterAdd().x - i == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetAllDireValue(),
					kCanMoveDirection::left);
			}
		}
	}

	// rightまでの距離
	for (int i = 1; i < cow2rightWall_; i++) {
		// leftまでの距離で牛飼いがいたら
		if (cow_->GetCenterAdd().x + i == cowherd_->GetCenterAdd().x &&
			cow_->GetCenterAdd().y == cowherd_->GetCenterAdd().y) {
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetAllDireValue(),
				kCanMoveDirection::right);
		}

		// rightまでの距離で若人がいたら
		for (int j = 0; j < youngPerson_->GetYoungMaxIndex(); j++) {
			if (cow_->GetCenterAdd().x + i == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetAllDireValue(),
					kCanMoveDirection::right);
			}
		}
	}

}

/*=================================================================
	マス目ごとの距離の計算
=================================================================*/
void CowCollision::CheckGridDistance(const Vec2& add) {
	Vec2f cow2PlayerDis{};
	Vec2 naturalDis{};
	int directionValue[8]{ 0 };
	int keepDire[8]{ 0 };

	for (int i = 0; i < 8; i++) {
		/*directionValue[i] = cow_->GetGridDistanceValue(i);*/
	}

	// 牛飼いとのマス目上の距離を取る
	cow2PlayerDis.x = static_cast<float>(cow_->GetCenterAdd().x - add.x);
	cow2PlayerDis.y = static_cast<float>(cow_->GetCenterAdd().y - add.y);

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
			keepDire[i] = ((10 - std::clamp(directionValue[i], 0, 10)) * cow_->GetClampValue());
			/*keepDire[i] = ((10 - (directionValue[i])) * cow_->GetClampValue());*/

			cow_->SetGridDistanceValue(keepDire[i], i);
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(i) + keepDire[i], i);
		}
	}

}

void CowCollision::CheckGridDire(const Vec2& add) {
	switch (cow_->GetEvaluteGrid()[add.y][add.x]) {
	case direEvaluateGrid::grid_top:

		break;
	}
	
}