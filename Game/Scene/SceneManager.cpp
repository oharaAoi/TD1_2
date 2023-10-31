#include "SceneManager.h"
#include <Novice.h>

SceneManager::SceneManager() {

	sceneArr_[SCENE::TITLE] = std::make_unique<Scene_Title>();
	sceneArr_[SCENE::GAME] = std::make_unique<Scene_Game>();

	nowSceneNo_ = BaseScene::GetNowSceneNo();

}


SceneManager::~SceneManager() {}


void SceneManager::Run() {

	// シーンNoの更新処理
	BaseScene::SceneNoUpdate();
	// 今のシーンNoをnowSceneNoに格納
	nowSceneNo_ = BaseScene::GetNowSceneNo();

	// 今のシーンの更新処理
	sceneArr_[nowSceneNo_]->Update();

	// 今のシーンの描画処理
	sceneArr_[nowSceneNo_]->Draw();

	// 今のシーンNoをデバッグ表示
	Novice::ScreenPrintf(10, 10, "nowSceneNo = %d", nowSceneNo_);

}
