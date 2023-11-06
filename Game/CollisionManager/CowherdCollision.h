#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Cow.h"

// ----- math ----- //
#include <SafeDelete.h>
#include "CanMoveDirection.h"

// ----- lib ----- //
#include "MyMath.h"

class CowherdCollision final {

private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	Cow* cow_;

public:

	// Constructor & Destructor
	CowherdCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow);
	~CowherdCollision();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow);

	void Finalize();

	// user method
	void CowherdCanMove();

private:

	bool CowherdCheckCanMove(const Vec2& add);
	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};

