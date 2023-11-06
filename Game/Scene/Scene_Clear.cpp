#include "Scene_Clear.h"

Scene_Clear::Scene_Clear() {}

Scene_Clear::~Scene_Clear() {}

/*================================================================
	初期化関数
================================================================*/
void Scene_Clear::Init() {



}

/*================================================================
	更新処理関数
================================================================*/
void Scene_Clear::Update() {

	if (input->IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::TITLE;
	}

}

/*================================================================
	描画処理関数
================================================================*/
void Scene_Clear::Draw() {

	Novice::ScreenPrintf(10, 60, "GameClear");

}

void Scene_Clear::Finalize() {
}
