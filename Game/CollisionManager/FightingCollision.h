﻿#pragma once
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

	void PersonCollision(const Vec2& add, int index);

/*================================================================
	評価
================================================================*/

	// 犬との評価
	void CheckDogExist();

	// 岩との評価
	void CheckRockAdjoin();

	// 進行方向の評価(csvでやる)
	void CheckMoveDire(const Vec2& add);

	// 進行方向に岩があるか
	void CheckMoveDireRock(const Vec2& add);

	// 進行方向の最終的な評価
	void MoveDireDecision();

	// 5つのエリアの評価
	void CheckFiveAreas(const Vec2& add);

	// 5つのエリアの最終的な評価
	void FiveDireDecison();

	// 4つのエリアでの評価
	void CheckFourAreas();

	// 近い柵を調べる
	void CheckNearFence();

	// 進む方向で同値があるか調べる
	void CheckSameDireValue();

	void SameDireValue();

};

