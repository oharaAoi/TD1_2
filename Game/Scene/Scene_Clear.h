#pragma once
#include "BaseScene.h"
#include <Novice.h>

// ----- math ----- //
#include "InputManager.h"
#include <SafeDelete.h>

class Scene_Clear : public BaseScene {

private:

	// 入力処理のインスタンス確保
	InputManager* input = InputManager::GetInstance();

public:

	Scene_Clear();
	~Scene_Clear();

	// デフォルト関数のオーバーライド
	void Init() override; // 初期化
	void Update() override; // 更新
	void Draw() override; // 描画
	void Finalize() override; // 終了

};