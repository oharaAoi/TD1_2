#pragma once
// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Cow.h"
#include "Dog.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"


class CowCollision {

private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	Cow* cow_;
	Dog* dog_;

public:
	// Constructor & Destructor
	CowCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog);
	~CowCollision();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow, Dog* dog);
	
	void Finalize();

	// user method
	void CheckCowMoveDire();

	void CheckCollision();

private:

	// 評価で使う
	void CheckCowAdjoin();
	void CheckDogExist();
	void CheckCowFourArea();
	void CheckGridDistance(const Vec2& add);
	void CheckCowMoveAllDire();

};

