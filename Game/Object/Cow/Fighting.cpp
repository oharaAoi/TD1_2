#include "Fighting.h"

BullFighting::BullFighting() { Init(); }

BullFighting::~BullFighting() { Finalize(); }

void BullFighting::Finalize() {
}

/*========================================================
	初期化関数
========================================================*/
void BullFighting::Init() {

	// ワールド空間での中心座標
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::BULLFIGHTING) {
				worldPos_ = {
					col * tileSize_.x + (tileSize_.x * 0.5f),
					row * tileSize_.y + (tileSize_.y * 0.5f)
				};
			}
		}
	}

	moveGrid_ = LoadFile("./Resources/cow/fightingDireRange.csv");

	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {
			switch (moveGrid_[row][col]) {
			case kCanMoveDirection::top:
				direAddressNum_[kCanMoveDirection::top]++;
				break;

			case kCanMoveDirection::bottom:
				direAddressNum_[kCanMoveDirection::bottom]++;
				break;

			case kCanMoveDirection::left:
				direAddressNum_[kCanMoveDirection::left]++;
				break;

			case kCanMoveDirection::right:
				direAddressNum_[kCanMoveDirection::right]++;
				break;

			case 50:
				localCenterAdd_.x = col;
				localCenterAdd_.y = row;
				break;
			}
		}
	}

	// 各方向を調べる数を配列に入れる
	for (int i = 0; i < 4; i++) {
		cannotMove_[i].localAdd.resize(direAddressNum_[i]);
		cannotMove_[i].worldAdd.resize(direAddressNum_[i]);
	}

	CenterAddUpdate();

	// 移動状態
	isIdle_ = false;

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, tileSize_.y * 0.5f },
		{ -tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
		{ tileSize_.x * 0.5f, -tileSize_.y * 0.5f },
	};

	// ワールド行列の生成
	MakeWorldMatrix();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		nearWallOfValue_[i] = 0;
	}

	gh_ = Novice::LoadTexture("white1x1.png");

	// 移動方向/量

	moveScalar_.x = 3;
	moveScalar_.y = 3;

	//=======================================================================================
	// csvで方向の評価
	moveGrid_ = LoadFile("./Resources/cow/fightingDireRange.csv");



	//=======================================================================================
	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	for (int i = 0; i < 4; i++) {
		nearWallOfValue_[i] = 0;
	}

	// 評価した数値の最大値
	maxDireValue_ = 0;
	// その添え字
	maxDireValueIndex_ = 0;

	// 全方向囲まれている時
	adjoinNum_ = 0;

	// 最終的に動く方向
	movedDire_ = top;

	//=========================================
	// 評価値(ここの数値をいじればいける)
	value_.wall = 5;
	value_.fourArea = 5;
	value_.allDire = 15;
	value_.clamp = 2;

	value_.dog = 150;
	value_.rock = 1000;
	value_.adjoin = 150;
	value_.adjoinAdd = 200;
	value_.fence = 200;
	value_.slantFence = -200;

	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	isFenceAttack_ = false;
	//=======================================================================================
}

/*========================================================
	更新処理関数
========================================================*/
void BullFighting::Update() {
	// 各頂点のアドレスの計算
	CenterAddUpdate();

	// 牛のターンか
	MoveTurn();

	// 移動
	Move();

	//　移動後の各頂点のアドレスの計算
	CenterAddUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();

	DireInit();
}

/*========================================================
	描画処理関数
========================================================*/
void BullFighting::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFF00FF);

}

/*========================================================
	座標系関数
========================================================*/
void BullFighting::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void BullFighting::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);
}

/*========================================================
	処理関数
========================================================*/

void BullFighting::CenterAddUpdate() {
	worldAdd_ = {
		static_cast<int>(worldPos_.x / tileSize_.x),
		static_cast<int>(worldPos_.y / tileSize_.y)
	};

	int index[4] = { 0 };

	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {
			switch (moveGrid_[row][col]) {
			case kCanMoveDirection::top:
				cannotMove_[kCanMoveDirection::top].localAdd[index[0]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[0]++;
				break;

			case  kCanMoveDirection::bottom:
				cannotMove_[kCanMoveDirection::bottom].localAdd[index[1]] = { col - localCenterAdd_.x,(row - localCenterAdd_.y) };
				index[1]++;
				break;

			case  kCanMoveDirection::left:
				cannotMove_[kCanMoveDirection::left].localAdd[index[2]] = { (col - localCenterAdd_.x),row - localCenterAdd_.y };
				index[2]++;
				break;

			case  kCanMoveDirection::right:
				cannotMove_[kCanMoveDirection::right].localAdd[index[3]] = { col - localCenterAdd_.x,row - localCenterAdd_.y };
				index[3]++;
				break;
			}
		}
	}
	std::reverse(cannotMove_[kCanMoveDirection::bottom].localAdd.begin(), cannotMove_[kCanMoveDirection::bottom].localAdd.end());
	std::reverse(cannotMove_[kCanMoveDirection::left].localAdd.begin(), cannotMove_[kCanMoveDirection::left].localAdd.end());

	for (int dire = 0; dire < 4; dire++) {
		for (int i = 0; i < direAddressNum_[dire]; i++) {
			cannotMove_[dire].worldAdd[i] = cannotMove_[dire].localAdd[i] + worldAdd_;
		}
	}
}

void BullFighting::DireInit() {
	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}

	// 進行方向に人がいるか
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			isMoveDireBlock_[i][j] = false;
			moveDireOnPreson_[i][j] = false;
		}
	}
}

void BullFighting::MoveTurn() {
	if (isTurnChange_) {
		isIdle_ = true;
	}
}

void BullFighting::Move() {

	if (isIdle_) {
		maxDireValue_ = -99999;
		maxDireValueIndex_ = 0;
		adjoinNum_ = 0;

		//全方位囲まれていたら処理を終わる
		for (int i = 0; i < 8; i++) {
			if (canMoveDireValue_[i] < -900) {
				adjoinNum_++;
			}
		}

		if (adjoinNum_ == 8) {
			return;
		}

		// 一番値の大きい方向に動く
		for (int i = 0; i < 4; i++) {
			if (maxDireValue_ < canMoveDireValue_[i]) {
				maxDireValue_ = canMoveDireValue_[i];
				maxDireValueIndex_ = i;
			}
		}

		// 移動方向の量によって進む箇所を決める
		switch (maxDireValueIndex_) {
		case kCanMoveDirection::top:
			worldPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::top;
			break;

		case kCanMoveDirection::bottom:
			worldPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::bottom;
			break;

		case kCanMoveDirection::left:
			worldPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::left;
			break;

		case kCanMoveDirection::right:
			worldPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::right;
			break;

		}

		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;

		// 待機状態の解除
		isIdle_ = false;

		if (isFenceAttack_) {
			isFenceAttack_ = false;
		}
	}
}


