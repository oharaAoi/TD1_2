#pragma once

#include "BaseScene.h"

class Scene_Title : public BaseScene {
private:

	// このシーンに必要な変数や構造体を宣言; 初期化はInit()で行う


public:
	// コンストラクタ
	Scene_Title();
	// デストラクタ
	~Scene_Title();


	// デフォルト関数のオーバーライド
	void Init() override; // 初期化
	void Update() override; // 更新
	void Draw() override; // 描画
	void Finalize() override; // 終了


};

