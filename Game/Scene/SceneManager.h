#pragma once

#include <memory>

// ----- Scene ----- //
#include "BaseScene.h"
#include "Scene_Title.h"
#include "Scene_Game.h"


class SceneManager {
private:

	// 各シーン
	std::unique_ptr<BaseScene> sceneArr_[2];

	int nowSceneNo_;


public:
	// コンストラクタ
	SceneManager();
	~SceneManager();

	// シーンループ
	void Run();

};

