#include "Scene_Game.h"

Scene_Game::Scene_Game() { Init(); }

Scene_Game::~Scene_Game() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_Game::Init() {

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
}


/*================================================================
	更新処理関数
================================================================*/
void Scene_Game::Update() {

	// 次のシーンへの条件(仮)
	if (input->IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::TITLE;
	}

	// このシーンの初期化条件
	if (preSceneNo_ != SCENE::GAME) {
		Init();
	}

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

	// 牛の更新
	cow_->Update();

	// ----- Collision ----- //
	collisionManager_->CheckCanMove();

	// 牛の動ける方向のための更新
	if (turnManager_->GetIsTurnChange()) {
		collisionManager_->CheckCanCowMove();
	}

	// 動いた時の当たり判定
	collisionManager_->CheckCowCollison();

	// ----- MatrixChange ----- //
	ChangeMatrix();

	// ゲームクリア
	if (collisionManager_->CheckClear()) {
		nextSceneNo_ = SCENE::GAMECLEAR;
	}

}


/*================================================================
	描画処理関数
================================================================*/
void Scene_Game::Draw() {

	mapChip_->Draw();
	mapChip_->DebugDraw(); // マップチップのDraw関数に入れる

	// 牛の描画
	cow_->Draw();
	
	renderer_->Draw();
	turnManager_->Draw();
}


/*================================================================
	終了処理関数
================================================================*/
void Scene_Game::Finalize() {

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


void Scene_Game::ChangeMatrix() {


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

	// 犬の行列を変換
	dog_->MatrixChange(
		camera_->GetViewMatrix(),
		camera_->GetOrthoMatrix(),
		camera_->GetViewportMatrix()
	);

}
