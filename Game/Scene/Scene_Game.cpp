#include "Scene_Game.h"

Scene_Game::Scene_Game() { Init(); }

Scene_Game::~Scene_Game() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_Game::Init() {

	camera_ = new Camera();

	mapChip_ = new MapChip();
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
	camera_->Update();


	// ----- Collision ----- //



	// ----- MatrixChange ----- //
	mapChip_->MatrixChange(camera_->GetViewMatrix(), camera_->GetOrthoMatrix(), camera_->GetViewportMatrix());


}


/*================================================================
	描画処理関数
================================================================*/
void Scene_Game::Draw() {

	mapChip_->Draw();

}


/*================================================================
	終了処理関数
================================================================*/
void Scene_Game::Finalize() {

	SafeDelete(camera_);
	SafeDelete(mapChip_);
}
