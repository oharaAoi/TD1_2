#pragma once

#include "BaseScene.h"

// ----- math ----- //
#include "InputManager.h"
#include <SafeDelete.h>

// ----- object ----- //
#include "Camera.h"
#include "MapChip.h"
#include "Cow.h"
#include "Cowherd.h"

class Scene_Game : public BaseScene {
private:

	// このシーンに必要な変数や構造体を宣言; 初期化はInit()で行う
	
	// 入力処理のインスタンス確保
	InputManager* input = InputManager::GetInstance();

	// カメラ
	Camera* camera_;

	// 背景 & マップ上のオブジェクト
	MapChip* mapChip_;

	// 牛
	Cow* cow_;

	// 牛飼い
	Cowherd* cowherd_;


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


};

