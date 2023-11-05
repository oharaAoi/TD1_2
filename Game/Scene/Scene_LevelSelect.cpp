#include "Scene_LevelSelect.h"


Scene_LevelSelect::Scene_LevelSelect() { Init(); }

Scene_LevelSelect::~Scene_LevelSelect() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_LevelSelect::Init() {

	selectStage_ = 0;


}


/*================================================================
	更新処理関数
================================================================*/
void Scene_LevelSelect::Update() {

	if (input->IsTriggerKey(DIK_LEFT)) {
		if (selectStage_ > 0) {
			selectStage_--;
		}
	}

	if (input->IsTriggerKey(DIK_RIGHT)) {
		if (selectStage_ < 10) {
			selectStage_++;
		}
	}

}


/*================================================================
	描画処理関数
================================================================*/
void Scene_LevelSelect::Draw() {



}


/*================================================================
	終了処理関数
================================================================*/
void Scene_LevelSelect::Finalize() {



}