﻿#pragma once
#include <algorithm>

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Dog.h"
#include "BullCow.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"


class BullCollision
{

private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	BullCow* bullCow_;
	Dog* dog_;

public:

	BullCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog);
	~BullCollision();

	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog);
	void Finalize();

	void CheckBullMoveDire();

	void CheckRockCollision();

	void PersonCollision(const Vec2& add);

	void CheckFenceScissorsCollision();

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

private:

	// 評価で使う

	void CheckDogExist();

	void CheckGridDistance(const Vec2& add);

	void CheckBullCowMoveAllDire();

	void CheckFourAreas();

	void CheckFenseCollision();

	void CheckNearWall();

	void CheckBullCowAdjoin();

	void CheckGridDire(const Vec2& add);

	// 進む方向で同値があるか調べる
	void CheckSameDireValue();

	void SameDireValue();
};


