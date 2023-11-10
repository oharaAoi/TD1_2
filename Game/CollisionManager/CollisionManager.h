#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Cow.h"
#include "Dog.h"
#include "Fighting.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"

// ----- lib ----- //
#include "MyMath.h"

// ----- collision ----- //
#include "CowCollision.h"
#include "CowherdCollision.h"
#include "YoungPersonCollision.h"
#include "BullCollision.h"
#include "FightingCollision.h"

/// <summary>
/// 当たり判定と移動できるかの判定を取るクラス
/// </summary>
class CollisionManager final {
private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	Cow* cow_;
	BullCow* bull_;
	BullFighting* fighting_;
	Dog* dog_;

	CowCollision* cowCollision_;
	CowherdCollision* cowherdCollison_;
	YoungPersonCollision* youngPersonCollision_;
	BullCollision* bullCollision_;
	FightingCollision* fightingCollision_;

public:
	// Constructor & Destructor
	CollisionManager(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog, BullCow* bull, BullFighting* fighting);
	~CollisionManager();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog, BullCow* bull, BullFighting* fighting);
	//void Update();
	//void Draw();
	void Finalize();

	// user method
	void CheckCanMove();

	void CheckOverLapping();

	// 牛の動く方向の判定
	void CheckCanCowMove();

	// 雄牛の動く判定
	void CheckCanBullMove();

	// 闘牛の動く判定
	void CheckCanFigthingMove();

	void CheckCowCollison();
	void CheckBullCollision();
	void CheckFigthingCollision();

	// クリア条件の判定
	bool CheckClear();

private:

	// user method
	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};

