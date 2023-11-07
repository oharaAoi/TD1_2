#include "Cow.h"

Cow::Cow(MapChip* mapChip) { Init(mapChip); }

Cow::~Cow() { Finalize(); }


/*==========================================================
	初期化関数
==========================================================*/
void Cow::Init(MapChip* mapChip) {

	// ワールド空間での中心座標
	size_ = mapChip->GetTileSize();
	for (int row = 0; row < mapChip->GetMapChipRow(); row++) {
		for (int col = 0; col < mapChip->GetMapChipCol(); col++) {

			if (mapChip->GetMapChipAdd()[row][col] == ChipType::COW) {
				worldCenterPos_ = {
					col * mapChip->GetTileSize().x + (size_.x * 0.5f),
					row * mapChip->GetTileSize().y + (size_.y * 0.5f)
				};
			}
		}
	}

	worldPreCenterPos_ = worldCenterPos_;

	gh_ = Novice::LoadTexture("white1x1.png");

	isIdle_ = false;

	// ローカル空間での各頂点座標
	localVertex_ = {
		{ -size_.x * 0.5f, size_.y * 0.5f },
		{ size_.x * 0.5f, size_.y * 0.5f },
		{ -size_.x * 0.5f, -size_.y * 0.5f },
		{ size_.x * 0.5f, -size_.y * 0.5f },
	};

	MakeWorldMatrix();

	// ローカル空間以外の各行列
	screenMatrix_ = worldMatrix_;

	// ワールドとスクリーン空間での各頂点座標
	screenVertex_ = worldVertex_;

	// アドレスの初期化
	CenterAddUpdate();

	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	moveScalar_.x = 1;
	moveScalar_.y = 1;

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
	value_.wall = 3;
	value_.fourArea = 10;
	value_.allDire = 5;
	value_.clamp = 2;
	value_.dog = 1500;
	value_.adjoin = -1000;
	value_.fence = 1000;

	//=========================================
	//評価で使う
	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;
	wallIsDuplicate_ = false;

	//=========================================
	// csvを読み込んで評価するための変数の初期化
	evaluateGrid_ = LoadFile("./Resources/cow/cowEightDireRange.csv");

}


/*==========================================================
	更新処理関数
==========================================================*/
void Cow::Update() {

	// ターンが牛側でなければ早期returnする
	/*if (turnType_ != kTurnType::Cows) { return; }*/

	// 牛の現在の位置を取得
	CenterAddUpdate();

	// デバック用
	CowMove();

	// 実際に動く
	Move();

	// 移動後のアドレスを計算する
	CenterAddUpdate();

	// ワールド空間の行列と各頂点座標の計算
	MakeWorldMatrix();
}


/*==========================================================
	描画処理関数
==========================================================*/
void Cow::Draw() {

	Draw::Quad(screenVertex_, { 0.0f,0.0f }, { 1.0f,1.0f }, gh_, 0xFFFFFFFF);

	DebugScreenPrintf();

	ImguiDraw();
}

/*==========================================================
	終了処理関数
==========================================================*/
void Cow::Finalize() {


}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cow::CowMove() {
		if (input->IsTriggerKey(DIK_M) or isTurnChange_) {
			isIdle_ = true;
		}
}

void Cow::Move() {
	if (isIdle_) {
		// 残りの評価値を計算
		/*CheckNearPerson();*/
		CheckNearWall();

		maxDireValue_ = 0;
		maxDireValueIndex_ = 0;
		adjoinNum_ = 0;

		worldPreCenterPos_ = worldCenterPos_;

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
		for (int i = 0; i < 8; i++) {
			if (maxDireValue_ < canMoveDireValue_[i]) {
				maxDireValue_ = canMoveDireValue_[i];
				maxDireValueIndex_ = i;
			}
		}

		// 移動方向の量によって進む箇所を決める
		switch (maxDireValueIndex_) {
		case kCanMoveDirection::top:
			worldCenterPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::top;
			break;

		case kCanMoveDirection::bottom:
			worldCenterPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::bottom;
			break;

		case kCanMoveDirection::left:
			worldCenterPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::left;
			break;

		case kCanMoveDirection::right:
			worldCenterPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			movedDire_ = kCanMoveDirection::right;
			break;

		case kCanMoveDirection::leftTop:
			worldCenterPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			worldCenterPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftTop;
			break;

		case kCanMoveDirection::rightTop:
			worldCenterPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			worldCenterPos_.y += tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::rightTop;
			break;

		case kCanMoveDirection::leftBottom:
			worldCenterPos_.x -= tileSize_.x * static_cast<float>(moveScalar_.x);
			worldCenterPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftBottom;
			break;

		case kCanMoveDirection::rightBottom:
			worldCenterPos_.x += tileSize_.x * static_cast<float>(moveScalar_.x);
			worldCenterPos_.y -= tileSize_.y * static_cast<float>(moveScalar_.y);
			movedDire_ = kCanMoveDirection::leftBottom;
			break;
		}

		// 移動の終了とともにturnがプレイヤー側になる
		turnType_ = kTurnType::Players;

		// 待機状態の解除
		isIdle_ = false;

		/*DireInit();*/
	}
}

// ------ 今のアドレスの計算 ------ //
void Cow::CenterAddUpdate() {
	centerAdd_ = {
		static_cast<int>(worldCenterPos_.x / size_.x),
		static_cast<int>(worldCenterPos_.y / size_.y)
	};
}

// ------ 方向の初期化 ------ //
void Cow::DireInit() {
	// 移動方向/量
	moveDire_.x = 0.0f;
	moveDire_.y = 0.0f;

	moveValue_.x = 0.0f;
	moveValue_.y = 0.0f;

	// 牛が動く方向の評価値
	for (int i = 0; i < 8; i++) {
		canMoveDireValue_[i] = 100;
		gridDistanceValue_[i] = 0;
	}
}

void Cow::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

}

void Cow::MakeWorldMatrix() {

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

}

//	マス目の計算で求めたものをクランプで10の間に収める
void Cow::CheckNearPerson() {
	for (int i = 0; i < 8; i++) {
		if (gridDistanceValue_[i] != 0) {
			canMoveDireValue_[i] += (10 - std::clamp(gridDistanceValue_[i], 0, 10)) * value_.clamp;
		}
	}
}

// 一番近い壁までの計算
void Cow::CheckNearWall() {

	// 近い壁を計算する
	nearWallOfValue_[kCanMoveDirection::top] = row_ - centerAdd_.y - 1;
	nearWallOfValue_[kCanMoveDirection::bottom] = centerAdd_.y;

	nearWallOfValue_[kCanMoveDirection::left] = centerAdd_.x;
	nearWallOfValue_[kCanMoveDirection::right] = col_ - centerAdd_.x - 1;


	wallMin_ = 1000;
	wallMinIndex_ = 0;
	wallMinNum_ = 0;

	// 同値があった時のフラグ
	wallIsDuplicate_ = false;

	// 一番低い値を求める
	for (int i = 0; i < 4; i++) {
		if (wallMin_ > nearWallOfValue_[i]) {
			wallMin_ = nearWallOfValue_[i];
			wallMinIndex_ = i;

		} else if (wallMin_ == nearWallOfValue_[i]) {
			wallMinNum_++;
			wallIsDuplicate_ = true;
		}
	}

	// 同値がなかった時はそれぞれの辺だけで済む
	if (!wallIsDuplicate_) {
		switch (wallMinIndex_) {
		case kCanMoveDirection::top:
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

			break;

		case kCanMoveDirection::bottom:
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
			break;

		case kCanMoveDirection::left:
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			break;

		case kCanMoveDirection::right:
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
			break;
		}
	}

	/// 2つ同値があった時
	if (wallMinNum_ == 2) {
		if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::right] &&
			nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::left]) {

			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::left] &&
			nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::right]) {

			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::left] == nearWallOfValue_[kCanMoveDirection::top] &&
			nearWallOfValue_[kCanMoveDirection::left] == nearWallOfValue_[kCanMoveDirection::bottom]) {

			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::right] == nearWallOfValue_[kCanMoveDirection::top] &&
			nearWallOfValue_[kCanMoveDirection::right] == nearWallOfValue_[kCanMoveDirection::bottom]) {

			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;

		}

	} else if (wallMinNum_ == 1) {
		if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::left]) {
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::top] == nearWallOfValue_[kCanMoveDirection::right]) {
			canMoveDireValue_[kCanMoveDirection::top] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightTop] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::left]) {
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::left] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::leftBottom] += value_.wall;

		} else if (nearWallOfValue_[kCanMoveDirection::bottom] == nearWallOfValue_[kCanMoveDirection::right]) {
			canMoveDireValue_[kCanMoveDirection::bottom] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::right] += value_.wall;
			canMoveDireValue_[kCanMoveDirection::rightBottom] += value_.wall;
		}
	}
}

void Cow::DebugScreenPrintf() {

	Novice::ScreenPrintf(1000, 450, "Keys[DIK_M]:cowMove");

	for (int i = 0; i < 4; i++) {
		Novice::ScreenPrintf(1100, 30 + (i * 20), "nearWall:%d", nearWallOfValue_[i]);
	}
}

void Cow::ImguiDraw() {
	//リリースの時は消す
	ImGui::Begin("evaluateValue");

	ImGui::SliderInt("value_.wall:", &value_.wall, 0, 30);
	ImGui::SliderInt("value_.fourArea:", &value_.fourArea, 0, 30);
	ImGui::SliderInt("value_.allDire:", &value_.allDire, 0, 30);
	ImGui::SliderInt("value_.clamp:", &value_.clamp, 0, 5);
	ImGui::SliderInt("value_.dog:", &value_.dog, 1000, 1500);
	ImGui::SliderInt("value_.fence:", &value_.fence, -1000, 1000);
	ImGui::SliderInt("value_.adjoin:", &value_.adjoin, -1000, 1000);
	ImGui::SliderInt("moveScalar_.x:", &moveScalar_.x, 0, 5);
	ImGui::SliderInt("moveScalar_.y:", &moveScalar_.y, 0, 5);

	ImGui::End();

	//
	ImGui::Begin("canMoveDireValue");

	for (int dire = 0; dire < 8; dire++) {
		ImGui::Text("%d : %s", canMoveDireValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::End();

	//
	ImGui::Begin("gridDistanceValue_");
	ImGui::BeginChild(ImGuiWindowFlags_NoTitleBar);

	for (int dire = 0; dire < 8; dire++) {
		ImGui::Text("%d : %s", gridDistanceValue_[dire], kCanMoveDire[dire]);
	}

	ImGui::EndChild();
	ImGui::End();

}
