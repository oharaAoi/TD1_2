#pragma once

#include "BaseScene.h"

// ----- math ----- //
#include "InputManager.h"
#include <SafeDelete.h>
#include "CollisionManager.h"

// ----- object ----- //
#include "Camera.h"
#include "MapChip.h"
#include "Cow.h"
#include "Cowherd.h"
#include "YoungPerson.h"
#include "Dog.h"

#include "Renderer.h"


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

	// 若人
	YoungPerson* youngPerson_;

	// 牛飼い
	Cowherd* cowherd_;

	// 犬
	Dog* dog_;

	/// 当たり判定
	CollisionManager* collisionManager_;

	// 描画
	Renderer* renderer_;

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

	void ChangeMatrix();

};

