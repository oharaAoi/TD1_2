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
	collisionManager_ = new CollisionManager(
		cowherd_, youngPerson_, mapChip_, cow_
	);

}


/*================================================================
	更新処理関数
================================================================*/
void Scene_Game::Update() {

	// 次のシーンへの条件(仮)
	if (input->IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::TITLE;
	}

	// ----- Update ----- //

	// カメラの更新
	camera_->Update();
	
	// 牛飼いの更新
	cowherd_->Update();

	// 若人の更新
	youngPerson_->Update();

	// 牛の更新
	cow_->Update();



	// ----- Collision ----- //
	collisionManager_->CheckCanMove();


	// ----- MatrixChange ----- //

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
}


/*================================================================
	描画処理関数
================================================================*/
void Scene_Game::Draw() {

	mapChip_->Draw();
	mapChip_->DebugDraw(); // マップチップのDraw関数に入れる

	// 牛の描画
	cow_->Draw();

	// 牛飼いの描画
	cowherd_->Draw();

	// 若人の描画
	youngPerson_->Draw();

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
	SafeDelete(collisionManager_);
}
