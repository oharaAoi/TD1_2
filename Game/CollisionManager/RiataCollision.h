﻿#pragma once

// ----- object ----- //
#include "Cow.h"
#include "Cowherd.h"
#include "YoungPerson.h"
#include "MapChip.h"
#include "BullCow.h"
#include "Fighting.h"

#include "Riata.h"

class RiataCollision {
private: // member objcet

	Riata* riata_;

	MapChip* mapChip_;
	Cow* cow_;
	BullCow* bullCow_;
	BullFighting* bullFighting_;
	Cowherd* cowherd_;
	YoungPerson* youngPerson_;


public: // member method
	// Constructor & Destructor
	RiataCollision(
		Riata* riata,
		MapChip* mapChip,
		Cow* cow,
		BullCow* bullCow,
		BullFighting* bullFighting,
		Cowherd* cowherd,
		YoungPerson* youngPerson
	);
	~RiataCollision();

	// ----- default method ----- //
	void Init(
		Riata* riata,
		MapChip* mapChip,
		Cow* cow,
		BullCow* bullCow,
		BullFighting* bullFighting,
		Cowherd* cowherd,
		YoungPerson* youngPerson
	);
	void Update();
	//void Draw();
	void Finalize();

	// ----- user method ----- //
	bool IsStop(const Vec2& address);
	bool IsEqualAdd(const Vec2& add1, const Vec2& add2);

};

