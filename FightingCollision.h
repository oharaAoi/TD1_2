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


/*================================================================
	ï]âø
================================================================*/

	// ä‚Ç∆ÇÃï]âø
	void CheckRockAdjoin();

	// êiçsï˚å¸ÇÃï]âø(csvÇ≈Ç‚ÇÈ)
	void CheckMoveDire();

	// 4Ç¬ÇÃÉGÉäÉAÇ≈ÇÃï]âø
	void CheckFourAreas();



};

