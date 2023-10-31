#include "BaseScene.h"


// 各静的メンバ変数の初期化
int BaseScene::nowSceneNo_ = SCENE::TITLE;
int BaseScene::preSceneNo_ = nowSceneNo_;
int BaseScene::nextSceneNo_ = preSceneNo_;


BaseScene::~BaseScene() {}

void BaseScene::SceneNoUpdate() {
	preSceneNo_ = nowSceneNo_;
	nowSceneNo_ = nextSceneNo_;
}
