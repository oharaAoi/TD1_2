#pragma once
#include "BaseScene.h"

// ----- math ----- //
#include "Inputs.h"
#include <SafeDelete.h>

class Scene_Clear : public BaseScene{

private:
	

public:

	Scene_Clear();
	~Scene_Clear();

	// デフォルト関数のオーバーライド
	void Init() override; // 初期化
	void Update() override; // 更新
	void Draw() override; // 描画
	void Finalize() override; // 終了
	

};

