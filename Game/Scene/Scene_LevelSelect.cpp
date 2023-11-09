#include "Scene_LevelSelect.h"
#include "MapChip.h"

int Scene_LevelSelect::selectStage_ = 0;

Scene_LevelSelect::Scene_LevelSelect() { Init(); }

Scene_LevelSelect::~Scene_LevelSelect() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_LevelSelect::Init() {



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
		if (selectStage_ < MapChip::GetKMaxStageNo()) {
			selectStage_++;
		}
	}

	if (input->IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::GAME;
	}

}


/*================================================================
	描画処理関数
================================================================*/
void Scene_LevelSelect::Draw() {

	Novice::ScreenPrintf(100, 100, " <- change level ->");
	Novice::ScreenPrintf(100, 120, "select level : %d", selectStage_);

}


/*================================================================
	終了処理関数
================================================================*/
void Scene_LevelSelect::Finalize() {



}