#pragma once
#include <algorithm>

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Dog.h"
#include "Fighting.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"


class FightingCollision{

private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	BullFighting* fighting_;
	Dog* dog_;

public:


	FightingCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog);
	~FightingCollision();

/*================================================================
	 default method
================================================================*/
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog);
	void Finalize();


/*================================================================
	user method
================================================================*/
	void CheckCanMoveDire();

	void CheckCollision();

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

/*================================================================
	当たり判定
================================================================*/

	void RockCollision();

	void FenceCollision();

	void PersonCollision(const Vec2& add);

/*================================================================
	評価
================================================================*/

	// 岩との評価
	void CheckRockAdjoin();

	// 進行方向の評価(csvでやる)
	void CheckMoveDire(const Vec2& add);

	void CheckMoveDireRock(const Vec2& add);

	// 進行方向の最終的な評価
	void MoveDireDecision();

	// 4つのエリアでの評価
	void CheckFourAreas();

	// 近い柵を調べる
	void CheckNearFence();

};

