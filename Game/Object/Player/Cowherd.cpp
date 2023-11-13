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
	destinationPos_ = worldCenterPos_; // 移動先
	startingPos_ = worldCenterPos_; // 移動元

	CenterAddUpdate();
	white1x1GH_ = Novice::LoadTexture("white1x1.png");
	scale_ = { 1.0f,1.0f };

	// ローカル座標系の頂点
	localVertex_.lt = { -size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.rt = { size_.x * 0.5f, size_.y * 0.5f };
	localVertex_.lb = { -size_.x * 0.5f, -size_.y * 0.5f };
	localVertex_.rb = { size_.x * 0.5f, -size_.y * 0.5f };

	riata_ = new Riata();

	/*=========================================
		移動に使う変数
	=========================================*/

	// 移動マスのロード
	moveGrid_ = LoadFile("./Resources/player/cowherdMoveValue.csv");

	moveGridMaxIndex_ = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			// アドレスに対応した初期化
			switch (moveGrid_[row][col]) {
			case Cowherd::Player:

				localCenterAdd_ = { col, row };
				break;
			case Cowherd::CanMove:

				// 移動マスの数をインクリメント
				moveGridMaxIndex_++;
				break;
			}
		}
	}

	// 移動マスの数に応じて要素数を変える
	canMoveGrid_.resize(moveGridMaxIndex_);

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
				canMoveGrid_[index].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f } , 0.0f , canMoveGrid_[index].worldCenterPos);
				canMoveGrid_[index].screenMatrix = canMoveGrid_[index].worldMatrix;

				canMoveGrid_[index].canMove = false;

				index++;


				break;
			}

		}

	}


	// ワールド空間で行列を作成
	MakeWorldMatrix();

	screenMatrix_ = worldMatrix_;
	screenVertex_ = worldVertex_;

	isMove_ = false;
	movingTime_ = 0.0f;
	isMoveIdle_ = false;

	// レイヤーの大きさ
	SetZOder(10);


}


/*==========================================================
	更新処理
==========================================================*/
void Cowherd::Update() {


	if (turnType_ != kTurnType::Players) { return; }

	if (isTurnChange_) {
		isMove_ = false;
	}

	// 投げ縄の更新
	riata_->Update();

	// 移動処理
	Move();
	// アドレスの更新
	CenterAddUpdate();
	// ワールド空間を構築
	MakeWorldMatrix();


}


/*==========================================================
	描画処理
==========================================================*/
void Cowherd::Draw() {

	// 移動マス
	for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

		// 移動できるマス; このターン動いていない; 動いていない; 移動待機状態
		if (canMoveGrid_[gi].canMove
			//&& !ch_isMove_
			&& !isMove_
			&& isMoveIdle_) {

			Draw::Quad(
				canMoveGrid_[gi].screenVertex ,
				{ 0.0f,0.0f } ,
				{ 1.0f,1.0f } ,
				white1x1GH_ ,
				0x00FFFF80
			);
		}
	}

	// 牛飼い
	Draw::Quad(
		screenVertex_ ,
		{ 128,0.0f } ,
		{ 64.0f,64.0f } ,
		GH_ ,
		0xFFFFFFFF
	);

	// 投げ縄の描画
	riata_->Draw();

	DebugDraw();
}


/*==========================================================
	終了処理
==========================================================*/
void Cowherd::Finalize() {
	SafeDelete(riata_);
}


/*==========================================================
	その他メンバ関数
==========================================================*/

void Cowherd::MatrixChange(const Matrix3x3& viewMatrix , const Matrix3x3& orthoMatrix , const Matrix3x3& viewportMatrix) {

	// 牛飼いをスクリーン空間に
	screenMatrix_ = MakeWvpVpMatrix(worldMatrix_ , viewMatrix , orthoMatrix , viewportMatrix);
	screenVertex_.lt = Transform(localVertex_.lt , screenMatrix_);
	screenVertex_.rt = Transform(localVertex_.rt , screenMatrix_);
	screenVertex_.lb = Transform(localVertex_.lb , screenMatrix_);
	screenVertex_.rb = Transform(localVertex_.rb , screenMatrix_);

	// 移動マスをスクリーン空間に
	for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

		canMoveGrid_[gi].screenMatrix = MakeWvpVpMatrix(canMoveGrid_[gi].worldMatrix , viewMatrix , orthoMatrix , viewportMatrix);
		canMoveGrid_[gi].screenVertex.lt = Transform(canMoveGrid_[gi].localVertex.lt , canMoveGrid_[gi].screenMatrix);
		canMoveGrid_[gi].screenVertex.rt = Transform(canMoveGrid_[gi].localVertex.rt , canMoveGrid_[gi].screenMatrix);
		canMoveGrid_[gi].screenVertex.lb = Transform(canMoveGrid_[gi].localVertex.lb , canMoveGrid_[gi].screenMatrix);
		canMoveGrid_[gi].screenVertex.rb = Transform(canMoveGrid_[gi].localVertex.rb , canMoveGrid_[gi].screenMatrix);
	}

	riata_->MakeScreenMatrix(
		viewMatrix ,
		orthoMatrix ,
		viewportMatrix
	);

}

void Cowherd::MakeWorldMatrix() {
	worldMatrix_ = MakeAffineMatrix(scale_ , 0.0f , worldCenterPos_);

	worldVertex_.lt = Transform(localVertex_.lt , worldMatrix_);
	worldVertex_.rt = Transform(localVertex_.rt , worldMatrix_);
	worldVertex_.lb = Transform(localVertex_.lb , worldMatrix_);
	worldVertex_.rb = Transform(localVertex_.rb , worldMatrix_);

	// 移動マス
	for (int i = 0; i < moveGridMaxIndex_; i++) {
		canMoveGrid_[i].worldMatrix = MakeAffineMatrix({ 1.0f,1.0f } , 0.0f , canMoveGrid_[i].worldCenterPos);
	}

}


void Cowherd::Move() {
	// 移動マス配列

	//isMove_ = false;
	if (isMoveIdle_) {

		// 移動中
		if (isMove_) {
			//isMoveIdle_ = false;
			if (movingTime_ < 60) {
				movingTime_++;
			}

			worldCenterPos_.x = MyMath::Lerp(Ease::InOut::Quint(movingTime_ / 60.0f) , startingPos_.x , destinationPos_.x);
			worldCenterPos_.y = MyMath::Lerp(Ease::InOut::Quint(movingTime_ / 60.0f) , startingPos_.y , destinationPos_.y);

			if (movingTime_ / 60.0f <= 0.5f) {

				scale_.x = MyMath::Lerp(Ease::Out::Quint(movingTime_ / 60.0f) , 1.0f , 2.0f);
				scale_.y = MyMath::Lerp(Ease::Out::Quint(movingTime_ / 60.0f) , 1.0f , 2.0f);

			} else {

				scale_.x = MyMath::Lerp(Ease::In::Quint(movingTime_ / 60.0f) , 2.0f , 1.0f);
				scale_.y = MyMath::Lerp(Ease::In::Quint(movingTime_ / 60.0f) , 2.0f , 1.0f);

			}

			// 移動の終了条件
			if (movingTime_ / 60.0f >= 1.0f) {
				isMove_ = false;
				ch_isMove_ = true;
				isMoveIdle_ = false;
				SetZOder(10);

			}


		} else { // 移動していない

			IsMoveIdleReSet();

			// 上下左右と斜めのマスとマウスの当たり判定をとりどこをクリックしたかで移動先を決める
			if (Inputs::IsTriggerMouse(0)) {
				if (!ch_isMove_) {

					// 移動マスの配列
					for (int gi = 0; gi < moveGridMaxIndex_; gi++) {

						if (canMoveGrid_[gi].canMove) {

							if (Collision::Rect::Point(
								canMoveGrid_[gi].screenVertex ,
								{ static_cast<float>(Inputs::GetMousePos().x),static_cast<float>(Inputs::GetMousePos().y) })) {

								// 移動先と移動元の座標の更新
								destinationPos_ = {
									worldCenterPos_.x + (canMoveGrid_[gi].localAdd.x * tileSize_.x),
									worldCenterPos_.y + (canMoveGrid_[gi].localAdd.y * tileSize_.y)
								};
								startingPos_ = worldCenterPos_;

								isMove_ = true;
								movingTime_ = 0;
								SetZOder(15);

								// 投げ縄の移動を開始
								RiataIsStartTrue();

								Stack::PushDate(nowMapAdd_);
								Stack::PushCH_isMovingCount(0);

								std::swap(
									nowMapAdd_[static_cast<int>(destinationPos_.y / tileSize_.y)][static_cast<int>(destinationPos_.x / tileSize_.x)] ,
									nowMapAdd_[static_cast<int>(startingPos_.y / tileSize_.y)][static_cast<int>(startingPos_.x / tileSize_.x)]
								);

							}
						}
					}
				}
			}

			// 投げ縄の待機フラグをtrueに
			RiataIsIdleTrue();

		}


	} else {


		if (Inputs::IsTriggerMouse(0)) {

			if (!ch_isMove_) {

				if (Collision::Rect::Point(screenVertex_ ,
					{ static_cast<float>(Inputs::GetMousePos().x),
					static_cast<float>(Inputs::GetMousePos().y) })) {

					isMoveIdle_ = true;

				}
			}
		}

	}

}

void Cowherd::IsMoveIdleReSet() {
	// いつでも移動待機状態をクリアできるようにする
	if (Inputs::IsTriggerMouse(1)) {
		isMoveIdle_ = false;
	}

	if (Inputs::IsTriggerMouse(0)) {
		for (int gi = 0; gi < moveGridMaxIndex_; gi++) {
			if (canMoveGrid_[gi].canMove) {
				if (Collision::Rect::Point(
					canMoveGrid_[gi].screenVertex ,
					{ static_cast<float>(Inputs::GetMousePos().x),static_cast<float>(Inputs::GetMousePos().y) })) {
					return;
				}
			}

			if (gi >= moveGridMaxIndex_ - 1) { isMoveIdle_ = false; }
		}
	}

}

void Cowherd::DebugDraw() {

	Novice::ScreenPrintf(100 , 100 , "isMoveIdle = %d" , isMoveIdle_);

	Novice::ScreenPrintf(100 , 120 , "centerAdd : x = %d, y = %d" , centerAdd_.x , centerAdd_.y);


}

void Cowherd::CenterAddUpdate() {

	// 牛飼いの中心アドレスを更新
	centerAdd_ = {
		static_cast<int>(worldCenterPos_.x / size_.x),
		static_cast<int>(worldCenterPos_.y / size_.y)
	};

	// 移動マス; 各マスの中心アドレスと座標
	int index = 0;
	for (int row = 0; row < moveGrid_.size(); row++) {
		for (int col = 0; col < moveGrid_[0].size(); col++) {

			// アドレスに対応した初期化
			switch (moveGrid_[row][col]) {
			case Cowherd::CanMove:

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

void Cowherd::RiataIsIdleTrue() {
	if (Inputs::IsTriggerKey(DIK_Q)) {
		riata_->SetIsIdle(true);
		riata_->SetWorldPos(worldCenterPos_);
		riata_->AddressUpdate();
	}
}

void Cowherd::RiataIsStartTrue() {
	riata_->SetStartingPos(worldCenterPos_);


	riata_->SetMoveDir(Vec2f{ 0.0f,0.0f });
	Vec2f tmp = MyMath::Direction(startingPos_ , destinationPos_ , MyMath::Normalized);
	if (tmp.x > 0) {
		riata_->SetMoveDir({ 1.0f, riata_->GetMoveDir().y });
	} else if (tmp.x < 0) {
		riata_->SetMoveDir({ -1.0f, riata_->GetMoveDir().y });
	}

	if (tmp.y > 0) {
		riata_->SetMoveDir({ riata_->GetMoveDir().x , 1.0f });
	} else if (tmp.y < 0) {
		riata_->SetMoveDir({ riata_->GetMoveDir().x , -1.0f });
	}

	//riata_->SetIsStart(true);
	riata_->AddressUpdate();
}
