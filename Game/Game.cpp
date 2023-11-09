#include "Game.h"

Game::Game() { Init(); }

Game::~Game() { Finalize(); }


/*========================================================
	初期化関数
========================================================*/
void Game::Init() {
	camera_ = new Camera();
	mapChip_ = new MapChip();
	cow_ = new Cow();
	cowherd_ = new Cowherd();
	youngPerson_ = new YoungPerson();
	dog_ = new Dog();
	bull_ = new BullCow();
	fighting_ = new BullFighting();

	collisionManager_ = new CollisionManager(
		cowherd_, youngPerson_, mapChip_, cow_, dog_, bull_
	);
	renderer_ = new Renderer();

	renderer_->AddDrawable(cowherd_);
	renderer_->AddDrawable(youngPerson_);
	renderer_->AddDrawable(dog_);


	turnManager_ = new TurnManager();

	// スタックの中身がなくなるまでPopする
	while (!Stack::GetEmpty()) {
		Stack::StackPop();
	}
	BaseMap::SetNowMapAdd(BaseMap::GetMapAdd());
	Stack::PushDate(BaseMap::GetNowMapAdd());


	isGameClear_ = false;
}


/*========================================================
	更新処理関数
========================================================*/
void Game::Update() {

	// ----- Update ----- //

	// Ctrl+Z
	BackOnce();


	// カメラの更新
	camera_->Update();

	// プレイヤーの更新
	cowherd_->Update();
	youngPerson_->Update();

	// 犬の更新
	dog_->Update();

	turnManager_->Update();

	// 牛の更新処理と当たり判定
	switch (mapChip_->GetCowType()) {
	case kCowType::Calf:
		// 牛の動ける方向のための更新(デバック用
		if (Inputs::IsTriggerKey(DIK_M)) {
			collisionManager_->CheckCanCowMove();
			collisionManager_->CheckCowCollison();
		}

		// 牛の動ける方向のための更新
		if (turnManager_->GetIsTurnChange()) {
			collisionManager_->CheckCanCowMove();
		}

		// クリア時に動く対策
		if (!isGameClear_) {
			// 牛の更新
			cow_->Update();
		}

		// 動いた時の当たり判定
		collisionManager_->CheckCowCollison();

		break;

	case kCowType::Bull:

		// 牛の動ける方向のための更新
		if (turnManager_->GetIsTurnChange()) {
			collisionManager_->CheckCanBullMove();

		}

		bull_->Update();

		collisionManager_->CheckBullCollision();

		break;
	case kCowType::Fighting:

		break;
	}


	// ターンの切り替わり時にスタックしていたものを解放する
	if (turnManager_->GetIsTurnChange()) {
		while (!Stack::GetEmpty()) {
			Stack::StackPop();
		}
	}


	// ----- Collision ----- //
	collisionManager_->CheckCanMove();

	// ----- MatrixChange ----- //
	ChangeMatrix();

	// ゲームクリア
	if (collisionManager_->CheckClear()) {
		isGameClear_ = true;
	}

}


/*========================================================
	描画処理関数
========================================================*/
void Game::Draw() {

	mapChip_->Draw();
	mapChip_->DebugDraw(); // マップチップのDraw関数に入れる

	// 牛の描画
	switch (mapChip_->GetCowType()) {
	case kCowType::Calf:
		cow_->Draw();
		break;

	case kCowType::Bull:
		bull_->Draw();
		break;

	case kCowType::Fighting:
		fighting_->Draw();
		break;
	}

	renderer_->Draw();
	turnManager_->Draw();

}


/*========================================================
	終了処理関数
========================================================*/
void Game::Finalize() {

	SafeDelete(camera_);
	SafeDelete(mapChip_);
	SafeDelete(cow_);
	SafeDelete(fighting_);
	SafeDelete(bull_);
	SafeDelete(cowherd_);
	SafeDelete(youngPerson_);
	SafeDelete(dog_);
	SafeDelete(collisionManager_);
	SafeDelete(renderer_);
	SafeDelete(turnManager_);

}


void Game::ChangeMatrix() {

	// マップの行列を変換
	mapChip_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	// 牛飼いの行列を変換
	cowherd_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	// 若人の行列を変換
	youngPerson_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	// 牛の行列を変換
	cow_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	// 雄牛の行列を変換
	bull_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	fighting_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

	// 犬の行列を変換
	dog_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);


}


void Game::BackOnce() {
	if (CheckIsMovingObject()) { return; }

	if (!Stack::GetEmpty()) {
		if (Inputs::IsTriggerKey(DIK_R)) {
			int yp_index = 0;
			for (int r = 0; r < mapChip_->GetMapChipRow(); r++) {
				for (int c = 0; c < mapChip_->GetMapChipCol(); c++) {

					switch (Stack::GetMapAdd()[r][c]) {
						/*case ChipType::NONE:
							break;*/
					case ChipType::STAGEOUT:
						break;
					case ChipType::FENCE:
						break;
					case ChipType::COWHERD:

						if (r != cowherd_->GetCenterAdd().y
							or c != cowherd_->GetCenterAdd().x) {

							BaseMap::Swap(
								cowherd_->GetCenterAdd(),
								{ c,r }
							);

							cowherd_->SetWorldPos(
								{ c * mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
								r * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f) }
							);
							cowherd_->SetCh_isMove(false);

						}

						break;
					case ChipType::YANGMAN:

						if (r != youngPerson_->GetCenterAdd(yp_index).y
							or c != youngPerson_->GetCenterAdd(yp_index).x) {

							BaseMap::Swap(
								youngPerson_->GetCenterAdd(yp_index),
								{ c,r }
							);

							youngPerson_->SetWorldPos(
								{ c * mapChip_->GetTileSize().x + (mapChip_->GetTileSize().x * 0.5f),
								r * mapChip_->GetTileSize().y + (mapChip_->GetTileSize().y * 0.5f) },
								yp_index
							);

							youngPerson_->SetYoung_IsMoveIdle(false, yp_index);
							youngPerson_->SetYP_IsMove(false, yp_index);
						}

						yp_index++;
						break;
					case ChipType::ROCK:
						break;
					case ChipType::COW:
						break;
					case ChipType::BULL:
						break;
					case ChipType::BULLFIGHTING:
						break;
					}

				}

			}

			Stack::StackPop();

		}
	}

}


bool Game::CheckIsMovingObject() {
	if (cowherd_->GetIsMove()) { return true; }
	for (int yi = 0; yi < youngPerson_->GetYoungMaxIndex(); yi++) {
		if (youngPerson_->GetIsMove(yi)) { return true; }
	}

	return false;
}
