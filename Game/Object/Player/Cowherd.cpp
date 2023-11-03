#include "Cowherd.h"

Cowherd::Cowherd() { Init(); }

Cowherd::~Cowherd() { Finalize(); }

/*==========================================================
	メンバ変数の初期化関数
==========================================================*/
void Cowherd::Init() {

	// ワールド空間での中心点
	size_ = tileSize_;
	for (int row = 0; row < row_; row++) {
		for (int col = 0; col < col_; col++) {

			if (mapAdd_[row][col] == ChipType::COWHERD) {
				worldCenterPos_ = {
					col * tileSize_.x + (size_.x * 0.5f),
					row * tileSize_.y + (size_.y * 0.5f)
				};
			}
		}
	}
	CenterAddUpdate();
	gh_ = Novice::LoadTexture("white1x1.png");

	// 各空間の頂点
	localVertex_.lt = { -size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.rt = { size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.lb = { -size_.x * 0.5f, -size_.y * 0.5f };
	localVertex_.rb = { size_.x * 0.5f, -size_.y * 0.5f };



	isMoveIdle_ = false;

	/*=========================================
		移動に使う変数
	=========================================*/

	// 移動方向
	moveDir_ = { 0.0f,0.0f };

	// 移動するマスの量
	moveValue_ = { 1.0f,1.0f };


	for (int i = 0; i < 8; i++) {
		canMoveDir_[i] = false;
	}

	moveGrid_ = LoadFile("./Resources/player/cowherdMoveValue.csv");
	std::reverse(moveGrid_.begin(), moveGrid_.end());

	maxIndex_ = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			// アドレスに対応した初期化
			switch (moveGrid_[row][col]) {
			case Cowherd::Player:

				localCenterAdd_ = { col, row };

				break;
			case Cowherd::CanMove:

				maxIndex_++;

				break;
			}
		}
	}

	// csv内の2の数分配列を変える
	canMoveGrid_.resize(maxIndex_);

	int index = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			// アドレスに対応した初期化
			switch (moveGrid_[row][col]) {
			case Cowherd::CanMove:

				// アドレス
				canMoveGrid_[index].localAdd = {
					col - localCenterAdd_.x,
					row - localCenterAdd_.y
				};

				canMoveGrid_[index].worldAdd = canMoveGrid_[index].localAdd + centerAdd_;

				// ワールド座標での中心
				canMoveGrid_[index].worldCenterPos = {
					canMoveGrid_[index].worldAdd.x * tileSize_.x + (size_.x * 0.5f),
					canMoveGrid_[index].worldAdd.y * tileSize_.y + (size_.y * 0.5f)
				};

				// 頂点座標(ローカル
				canMoveGrid_[index].localVertex = localVertex_;
				canMoveGrid_[index].screenVertex = canMoveGrid_[index].localVertex;

				// ワールド空間行列
				canMoveGrid_[index].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, canMoveGrid_[index].worldCenterPos);
				canMoveGrid_[index].screenMatrix = canMoveGrid_[index].worldMatrix;

				canMoveGrid_[index].canMove = false;

				index++;


				break;
			}

		}

	}


	// ローカル以外の各空間の行列
	MakeWorldMatrix();

	screenMatrix_ = worldMatrix_;
	screenVertex_ = worldVertex_;

}


/*==========================================================
	更新処理
==========================================================*/
void Cowherd::Update() {

	Move();
	CenterAddUpdate();
	MakeWorldMatrix();

}


/*==========================================================
	描画処理
==========================================================*/
void Cowherd::Draw() {

	Draw::Quad(
		screenVertex_,
		{ 128,0.0f },
		{ 64.0f,64.0f },
		GH_,
		0xFFFFFFFF
	);

	// 移動マス
	for (int i = 0; i < maxIndex_; i++) {
		Draw::Quad(
			canMoveGrid_[i].screenVertex,
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			gh_,
			0x00FFFF80
		);
	}


	DebugDraw();
}


/*==========================================================
	終了処理
==========================================================*/
void Cowherd::Finalize() {



}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cowherd::MatrixChange(const Matrix3x3& viewMatrix, const Matrix3x3& orthoMatrix, const Matrix3x3& viewportMatrix) {

	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_, viewMatrix, orthoMatrix, viewportMatrix);

	screenVertex_.lt = Transform(localVertex_.lt, screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt, screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb, screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb, screenMatrix_);

	// 移動マス
	for (int i = 0; i < maxIndex_; i++) {

		canMoveGrid_[i].screenMatrix = MakeWvpVpMatrix(canMoveGrid_[i].worldMatrix, viewMatrix, orthoMatrix, viewportMatrix);

		canMoveGrid_[i].screenVertex.lt = Transform(canMoveGrid_[i].localVertex.lt, canMoveGrid_[i].screenMatrix);
		canMoveGrid_[i].screenVertex.rt = Transform(canMoveGrid_[i].localVertex.rt, canMoveGrid_[i].screenMatrix);
		canMoveGrid_[i].screenVertex.lb = Transform(canMoveGrid_[i].localVertex.lb, canMoveGrid_[i].screenMatrix);
		canMoveGrid_[i].screenVertex.rb = Transform(canMoveGrid_[i].localVertex.rb, canMoveGrid_[i].screenMatrix);

	}

}

void Cowherd::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt, worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt, worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb, worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb, worldMatrix_);

	// 移動マス
	for (int i = 0; i < maxIndex_; i++) {

		canMoveGrid_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, canMoveGrid_[i].worldCenterPos);

	}

}


void Cowherd::Move() {

	if (isMoveIdle_) {

		// いつでも移動待機状態をクリアできるよにする
		if (input->IsTriggerMouse(1)) {
			isMoveIdle_ = false;
		}

		// 上下左右と斜めのマスとマウスの当たり判定をとりどこをクリックしたかで移動先を決める
		if (input->IsTriggerMouse(0)) {

			for (int i = 0; i < maxIndex_; i++) {

				if (Collision::Rect::Point(
					canMoveGrid_[i].screenVertex,
					{ static_cast<float>(input->GetMousePos().x),static_cast<float>(input->GetMousePos().y) })) {

					// ワールド座標の更新
					worldCenterPos_ += {
						canMoveGrid_[i].localAdd.x * tileSize_.x,
						canMoveGrid_[i].localAdd.y * tileSize_.y,
					};

				}
			}
		}



	} else {

		// スクリーン座標上でマウスの位置がオブジェクトの上にあれば当たっている判定になる
		if (Collision::Rect::Point(screenVertex_,
			{ static_cast<float>(input->GetMousePos().x), static_cast<float>(input->GetMousePos().y) })) {

			// クリックしたとき
			if (input->IsTriggerMouse(0)) {

				// 移動待機状態になる
				isMoveIdle_ = true;

				// 移動できる方向を所得


			}

		}

	}

}

void Cowherd::DebugDraw() {

	Novice::ScreenPrintf(100, 100, "isMoveIdle = %d", isMoveIdle_);

	Novice::ScreenPrintf(100, 120, "centerAdd : x = %d, y = %d", centerAdd_.x, centerAdd_.y);


}

void Cowherd::CenterAddUpdate() {
	centerAdd_ = {
		static_cast<int>(worldCenterPos_.x / size_.x),
		static_cast<int>(worldCenterPos_.y / size_.y)
	};

	int index = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			// アドレスに対応した初期化
			switch (moveGrid_[row][col]) {
			case Cowherd::CanMove:

				// アドレス
				canMoveGrid_[index].localAdd = {
					col - localCenterAdd_.x,
					row - localCenterAdd_.y
				};

				canMoveGrid_[index].worldAdd = canMoveGrid_[index].localAdd + centerAdd_;

				// ワールド座標での中心
				canMoveGrid_[index].worldCenterPos = {
					canMoveGrid_[index].worldAdd.x * tileSize_.x + (size_.x * 0.5f),
					canMoveGrid_[index].worldAdd.y * tileSize_.y + (size_.y * 0.5f)
				};

				index++;
				break;
			}

		}

	}
}
