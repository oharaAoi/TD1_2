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

	collisionManager_ = new CollisionManager(
		cowherd_, youngPerson_, mapChip_, cow_, dog_
	);
	renderer_ = new Renderer();

	renderer_->AddDrawable(cowherd_);
	renderer_->AddDrawable(youngPerson_);
	renderer_->AddDrawable(dog_);


	turnManager_ = new TurnManager();

	isGameClear_ = false;
}


/*========================================================
	更新処理関数
========================================================*/
void Game::Update() {

	// ----- Update ----- //

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

		break;
	}

	// ----- Collision ----- //
	collisionManager_->CheckCanMove();

	// 動いた時の当たり判定
	collisionManager_->CheckCowCollison();

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

	// 犬の行列を変換
	dog_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);


}

void Game::SetGame(Game* game) {
	camera_ = game->camera_;
	mapChip_ = game->mapChip_;
	cow_ = game->cow_;
	cowherd_ = game->cowherd_;
	youngPerson_ = game->youngPerson_;
	dog_ = game->dog_;
	bull_ = game->bull_;

	collisionManager_ = game->collisionManager_;
	renderer_ = game->renderer_;

	renderer_->ResetDrawable();
	renderer_->AddDrawable(cowherd_);
	renderer_->AddDrawable(youngPerson_);
	renderer_->AddDrawable(dog_);


	turnManager_ = game->turnManager_;

	isGameClear_ = game->isGameClear_;
}
