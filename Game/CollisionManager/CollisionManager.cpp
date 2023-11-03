#include "CollisionManager.h"

CollisionManager::CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	Init(cowherd, youngPerson, mapChip, cow);
}

CollisionManager::~CollisionManager() {
	
}


/*================================================================
	初期化関数
================================================================*/
void CollisionManager::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	cow_ = cow;
}


/*================================================================
	終了処理関数
================================================================*/
void CollisionManager::Finalize() {
	
}

/*=================================================================
	実質更新処理
=================================================================*/
void CollisionManager::CheckCanMove() {

	CowherdCanMove();
	YoungPersonCanMove();
	
}

void CollisionManager::CheckCowMoveDire(){
	CheckCowFourArea();
	CheckGridDistance(cowherd_->GetCenterAdd());
	CheckCowMoveAllDire();
}


/*=================================================================
	牛飼い
=================================================================*/

void CollisionManager::CowherdCanMove() {

	// 中心アドレスから上下左右、斜めの番地を確認
	// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない
	// 障害物がなければ移動できる

	if (cowherd_->GetIsMoveIdle()) {
		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x, cowherd_->GetCenterAdd().y + 1 })) { // 上
			cowherd_->SetCanMoveDir(true, top);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x, cowherd_->GetCenterAdd().y - 1 })) { // 下
			cowherd_->SetCanMoveDir(true, bottom);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y })) { // 左
			cowherd_->SetCanMoveDir(true, left);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y })) { // 右
			cowherd_->SetCanMoveDir(true, right);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y + 1 })) { // 左上
			cowherd_->SetCanMoveDir(true, leftTop);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y + 1 })) { // 右上
			cowherd_->SetCanMoveDir(true, rightTop);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x - 1, cowherd_->GetCenterAdd().y - 1 })) { // 左下
			cowherd_->SetCanMoveDir(true, leftBottom);
		}

		if (CowherdCheckCanMove({ cowherd_->GetCenterAdd().x + 1, cowherd_->GetCenterAdd().y - 1 })) { // 右下
			cowherd_->SetCanMoveDir(true, rightBottom);
		}
	}


}

bool CollisionManager::CowherdCheckCanMove(const Vec2& add) {
	// 動かない物はアドレス上で当たり判定を取る
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }

	// 動くものはアドレスが被っていないかで
	/*for (int yi = 0; yi < youngPerson_->GetIndexMax(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}*/

	return true;
}



/*=================================================================
	若人
=================================================================*/

void CollisionManager::YoungPersonCanMove() {

	//for (int yi = 0; yi < youngPerson_->GetIndexMax(); yi++) {

		//if (youngPerson_->GetIsMoveIdle(yi)) {

			// 中心アドレスから上下左右、斜めの番地を確認
			// 確認したアドレスに障害物およびプレイヤーがいれば移動ができない
			// 障害物がなければ移動できる

			//if (YoungPersonCheckCanMove(youngPerson_->GetMoveDirAdd(yi, top).add)) { // 上
			//	youngPerson_->SetCanMoveDir(true, top, yi);
			//}

			//if (YoungPersonCheckCanMove(youngPerson_->GetMoveDirAdd(yi, bottom).add)) { // 下
			//	youngPerson_->SetCanMoveDir(true, bottom, yi);
			//}

			//if (YoungPersonCheckCanMove(youngPerson_->GetMoveDirAdd(yi, left).add)) { // 左
			//	youngPerson_->SetCanMoveDir(true, left, yi);
			//}

			//if (YoungPersonCheckCanMove(youngPerson_->GetMoveDirAdd(yi, right).add)) { // 右
			//	youngPerson_->SetCanMoveDir(true, right, yi);
			//}

		//}

	//}

}

bool CollisionManager::YoungPersonCheckCanMove(const Vec2& add) {
	// マップ上のオブジェクト
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::FENCE) { return false; }
	if (mapChip_->GetMapChipAdd()[add.y][add.x] == ChipType::STAGEOUT) { return false; }

	// 牛との

	// 牛飼いとの
	if (IsEqualAdd(add, cowherd_->GetCenterAdd())) { return false; }

	// 若人どうしの; 上下左右のアドレスを取って計算するので添え字が同じもの動詞でも計算して大丈夫
	/*for (int yi = 0; yi < youngPerson_->GetIndexMax(); yi++) {
		if (IsEqualAdd(add, youngPerson_->GetCenterAdd(yi))) { return false; }
	}*/

	return true;
}

/*=================================================================
	牛
=================================================================*/

void CollisionManager::CheckCowFourArea() {
	// 左上エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis < mapChip_->GetMapChipRow(); yAxis++) {
		for (int xAxis = 0; xAxis < cow_->GetCenterAdd().x; xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - 2, kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - 2, kCanMoveDirection::leftTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - 2, kCanMoveDirection::top);
			}

			// 若人
			/*for (int i = 0; i < youngPerson_->GetIndexMax(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - 2, kCanMoveDirection::left);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftTop) - 2, kCanMoveDirection::leftTop);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - 2, kCanMoveDirection::top);
				}
			}*/
		}
	}

	// 右上エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis < mapChip_->GetMapChipRow(); yAxis++) {
		for (int xAxis = cow_->GetCenterAdd().x; xAxis < mapChip_->GetMapChipCol(); xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - 2, kCanMoveDirection::top);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightTop) - 2, kCanMoveDirection::rightTop);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - 2, kCanMoveDirection::right);
			}

			// 若人
			/*for (int i = 0; i < youngPerson_->GetIndexMax(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - 2, kCanMoveDirection::top);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - 2, kCanMoveDirection::right);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - 2, kCanMoveDirection::right);
				}
			}*/
		}
	}

	// 左下エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis > 0; yAxis--) {
		for (int xAxis = 0; xAxis < cow_->GetCenterAdd().x; xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - 2, kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - 2, kCanMoveDirection::left);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - 2, kCanMoveDirection::leftBottom);
			}

			// 若人
			/*for (int i = 0; i < youngPerson_->GetIndexMax(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - 2, kCanMoveDirection::bottom);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - 2, kCanMoveDirection::left);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::leftBottom) - 2, kCanMoveDirection::leftBottom);
				}
			}*/
		}
	}

	// 右下エリアを調べる
	for (int yAxis = cow_->GetCenterAdd().y; yAxis > 0; yAxis--) {
		for (int xAxis = cow_->GetCenterAdd().x; xAxis < mapChip_->GetMapChipCol(); xAxis++) {
			// 牛飼い
			if (cowherd_->GetCenterAdd().x == xAxis && cowherd_->GetCenterAdd().y == yAxis) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - 2, kCanMoveDirection::bottom);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - 2, kCanMoveDirection::right);
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - 2, kCanMoveDirection::rightBottom);
			}

			// 若人
			/*for (int i = 0; i < youngPerson_->GetIndexMax(); i++) {
				if (youngPerson_->GetCenterAdd(i).x == xAxis && youngPerson_->GetCenterAdd(i).y == yAxis) {
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - 2, kCanMoveDirection::bottom);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - 2, kCanMoveDirection::right);
					cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::rightBottom) - 2, kCanMoveDirection::rightBottom);
				}
			}*/
		}
	}
}

void CollisionManager::CheckCowMoveAllDire() {
	// 壁までの距離を計算
	int cow2topWall_ = mapChip_->GetMapChipRow() - cow_->GetCenterAdd().y;
	int cow2bottomWall_ = cow_->GetCenterAdd().y;
	int cow2leftWall_ = cow_->GetCenterAdd().x;
	int cow2rightWall_ = mapChip_->GetMapChipCol() - cow_->GetCenterAdd().x;

	// topまでの計算
	for (int i = 1; i < cow2topWall_; i++) {
		// topまでの距離で牛飼いがいたら
		if (cow_->GetCenterAdd().x == cowherd_->GetCenterAdd().x &&
			cow_->GetCenterAdd().y + i == cowherd_->GetCenterAdd().y) {
			cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetAllDireValue(),
									kCanMoveDirection::top);
		}

		// topまでの距離で若人がいたら
		/*for (int j = 0; j < youngPerson_->GetIndexMax(); j++) {
			if (cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y + i == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::top) - cow_->GetAllDireValue(),
					kCanMoveDirection::top);
			}
		}*/
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
		/*for (int j = 0; j < youngPerson_->GetIndexMax(); j++) {
			if (cow_->GetCenterAdd().x == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y - i == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::bottom) - cow_->GetAllDireValue(),
					kCanMoveDirection::bottom);
			}
		}*/
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
		/*for (int j = 0; j < youngPerson_->GetIndexMax(); j++) {
			if (cow_->GetCenterAdd().x - i == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::left) - cow_->GetAllDireValue(),
					kCanMoveDirection::left);
			}
		}*/
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
		/*for (int j = 0; j < youngPerson_->GetIndexMax(); j++) {
			if (cow_->GetCenterAdd().x + i == youngPerson_->GetCenterAdd(j).x &&
				cow_->GetCenterAdd().y == youngPerson_->GetCenterAdd(j).y) {
				cow_->SetMoveDireValue(cow_->GetMoveDireValue(kCanMoveDirection::right) - cow_->GetAllDireValue(),
					kCanMoveDirection::right);
			}
		}*/
	}

}

void CollisionManager::CheckGridDistance(const Vec2& add) {
	Vec2f cow2HeadDis{};
	Vec2 naturalDis{};
	int directionValue[8]{0};

	for (int i = 0; i < 8; i++) {
		directionValue[i] = cow_->GetGridDistanceValue(i);
	}

	// 牛飼いとのマス目上の距離を取る
	cow2HeadDis.x = static_cast<float>(cow_->GetCenterAdd().x - add.x);
	cow2HeadDis.y = static_cast<float>(cow_->GetCenterAdd().y - add.y);

	// 値を入れるときように-のものを+にする
	naturalDis.x = static_cast<int>(sqrtf(powf(cow2HeadDis.x, 2.0f)));
	naturalDis.y = static_cast<int>(sqrtf(powf(cow2HeadDis.y, 2.0f)));

	/*マイナスしているところは差が-値になっているから*/
	// 左右の量を計算
	if (cow2HeadDis.x > 0 && naturalDis.y == 0) {
		// 牛から見て左にいるため牛飼いが右に動く
		directionValue[kCanMoveDirection::right] += naturalDis.x;

	} else if(cow2HeadDis.x < 0 && naturalDis.y == 0) {
		// 牛から見て右にいるため牛飼いが左に動く
		directionValue[kCanMoveDirection::left] += naturalDis.x;
	}

	// 上下の量を計算
	if (cow2HeadDis.y < 0 && naturalDis.x == 0) {
		// 牛から見て上にいるため牛飼いが下に動く
		directionValue[kCanMoveDirection::bottom] += naturalDis.y;

	} else if(cow2HeadDis.y > 0 && naturalDis.x == 0){
		// 牛から見て下にいるため牛飼いが上に動く
		directionValue[kCanMoveDirection::top] += naturalDis.y;
	}


	// 左上下の計算
	// xとyで値が小さい方を斜めの量として計算
	if (cow2HeadDis.x > 0 && cow2HeadDis.y < 0) {
		// 左上
		// 牛から見て左上にいるため牛飼いが右下に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にyの量を、残りは右に動く
			directionValue[kCanMoveDirection::rightBottom] += naturalDis.y;
			directionValue[kCanMoveDirection::right] += naturalDis.x;

		} else if(naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にxの量を、残りは下に動く
			directionValue[kCanMoveDirection::rightBottom] +=naturalDis.x;
			directionValue[kCanMoveDirection::bottom] += naturalDis.y;

		} else {
			// 同じの場合はxとyを足して2で割った値斜めに足す
			directionValue[kCanMoveDirection::rightBottom] += ((naturalDis.x + naturalDis.y) / 2);
		}

	} else if(cow2HeadDis.x > 0 && cow2HeadDis.y > 0) {
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
			directionValue[kCanMoveDirection::rightBottom] += (naturalDis.x + naturalDis.y) / 2;
		}
	}

	// 右上下の計算
	if (cow2HeadDis.x < 0 && cow2HeadDis.y < 0) {
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
			directionValue[kCanMoveDirection::rightBottom] += (naturalDis.x + naturalDis.y) / 2;
		}

	}else if (cow2HeadDis.x < 0 && cow2HeadDis.y > 0) {
		// 右下
		// 牛から見て右下にいるため牛飼いが左上に動く
		if (naturalDis.x > naturalDis.y) {
			// yが小さいため斜めに動く分にy、残りは左に動く
			directionValue[kCanMoveDirection::leftTop] += naturalDis.y;
			directionValue[kCanMoveDirection::left] += naturalDis.x - naturalDis.y;

		} else if (naturalDis.x < naturalDis.y) {
			// xが小さいため斜めに動く分にx、残りは上に動く
			directionValue[kCanMoveDirection::leftTop] += naturalDis.x;
			directionValue[kCanMoveDirection::top] += naturalDis.y + naturalDis.x;

		} else {
			// 同じの場合はxとyを足して2で割った値を斜めに足す
			directionValue[kCanMoveDirection::rightBottom] += (naturalDis.x + naturalDis.y) / 2;
		}
	}

	for (int i = 0; i < 8; i++) {
		cow_->SetGridDistanceValue(directionValue[i], i);
	}

}


bool CollisionManager::IsEqualAdd(const Vec2& add1, const Vec2& add2) {
	return (add1.x == add2.x) && (add1.y == add2.y);
}
