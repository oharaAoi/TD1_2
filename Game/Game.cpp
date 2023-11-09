#include "Game.h"

Game::Game() { Init(); }

Game::~Game() { Finalize(); }


/*========================================================
	初期化関数
========================================================*/
void Game::Init() {
	camera_ = new Camera();
	mapChip_ = new MapChip();
	cow_ = new Cow(mapChip_);
	cowherd_ = new Cowherd();
	youngPerson_ = new YoungPerson();
	dog_ = new Dog();

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

	// 牛の動ける方向のための更新(デバック用
	if (input->IsTriggerKey(DIK_M)) {
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
	cow_->Draw();

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
}
