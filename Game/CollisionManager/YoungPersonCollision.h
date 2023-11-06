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


class YoungPersonCollision final {

private:

	Cowherd* cowherd_;
	YoungPerson* youngPerson_;
	MapChip* mapChip_;
	Cow* cow_;

public:

	// Constructor & Destructor
	YoungPersonCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow);
	~YoungPersonCollision();

	// default method
	void Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, Cow* cow);

	void Finalize();

	void YoungPersonCanMove();

private:

	// user method

	bool YoungPersonCheckCanMove(const Vec2& add);

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};

