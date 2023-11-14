#pragma once
// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "Cow.h"
#include "BullCow.h"
#include "Fighting.h"

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
	BullCow* bullCow_;
	BullFighting* fighting_;

public:

	// Constructor & Destructor
	YoungPersonCollision(
		Cowherd* cowherd,
		YoungPerson* youngPerson,
		MapChip* mapChip, 
		Cow* cow,
		BullCow* bullCow,
		BullFighting* fighting
	);
	~YoungPersonCollision();

	// default method
	void Init(Cowherd* cowherd,
		YoungPerson* youngPerson,
		MapChip* mapChip,
		Cow* cow,
		BullCow* bullCow,
		BullFighting* fighting
	);

	void Finalize();

	void YoungPersonCanMove();

	void YoungPresonIsStriked();

private:

	// user method

	bool YoungPersonCheckCanMove(const Vec2& add);

	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};

