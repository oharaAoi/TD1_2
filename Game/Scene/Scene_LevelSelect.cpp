﻿#include "Scene_LevelSelect.h"


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