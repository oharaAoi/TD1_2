#pragma once


// ----- math ----- //
#include "Inputs.h"
#include <SafeDelete.h>
#include "CollisionManager.h"

// ----- object ----- //
#include "Camera.h"
#include "MapChip.h"
#include "Cow.h"
#include "BullCow.h"
#include "Cowherd.h"
#include "YoungPerson.h"
#include "Dog.h"
#include "TurnManager.h"

#include "Renderer.h"

// ----- collision ----- //
#include "CowCollision.h"
#include "CowherdCollision.h"
#include "YoungPersonCollision.h"


class Game {
private: // member object

	// カメラ
	Camera* camera_;

	// 背景 & マップ上のオブジェクト
	MapChip* mapChip_;

	Cow* cow_;
	YoungPerson* youngPerson_;
	Cowherd* cowherd_;
	Dog* dog_;
	BullCow* bull_;

	/// 当たり判定
	CollisionManager* collisionManager_;

	// 描画
	Renderer* renderer_;

	TurnManager* turnManager_;

	bool isGameClear_;

public: // member method

	// Constructor & destructor
	Game();
	~Game();

	// default method
	void Init(); // 初期化
	void Update(); // 更新
	void Draw(); // 描画
	void Finalize(); // 終了


private: // user method

	void ChangeMatrix();


public: // accessor

	bool GetIsGameClear(void) const { return isGameClear_; }

	Cowherd* GetCowherd(void) { return cowherd_; }

	void SetGame(Game& game);

};

