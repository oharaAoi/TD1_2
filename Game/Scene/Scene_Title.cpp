﻿#include "Scene_Title.h"

Scene_Title::Scene_Title() { Init(); }

Scene_Title::~Scene_Title() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void Scene_Title::Init() {




}


/*================================================================
	更新処理関数
================================================================*/
void Scene_Title::Update() {

	if (Inputs::IsTriggerKey(DIK_RETURN)) {
		nextSceneNo_ = SCENE::LEVELSELECT;
	}


}


/*================================================================
	描画処理関数
================================================================*/
void Scene_Title::Draw() {

	// タイトルシーンの仮描画(後で消す
	Novice::DrawBox(640, 360, 32, 32, 0.0f, 0x00ddddff, kFillModeSolid);

}


/*================================================================
	終了処理関数
================================================================*/
void Scene_Title::Finalize() {


}
