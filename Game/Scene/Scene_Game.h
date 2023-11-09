#pragma once

#include "BaseScene.h"

// ----- math ----- //
#include "InputManager.h"
#include <SafeDelete.h>

#include "Game.h"
#include <stack>

class Scene_Game : public BaseScene {
private:

	InputManager* input = InputManager::GetInstance();

	Game* game_;

	std::stack<Game> sGame_;
	bool isBack_;
	int backCT_;

	

public:
	// コンストラクタ
	Scene_Game();
	// デストラクタ
	~Scene_Game();


	// デフォルト関数のオーバーライド
	void Init() override; // 初期化
	void Update() override; // 更新
	void Draw() override; // 描画
	void Finalize() override; // 終了

private:


};

