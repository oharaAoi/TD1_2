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
	�]��
================================================================*/

	// ��Ƃ̕]��
	void CheckRockAdjoin();

	// �i�s�����̕]��(csv�ł��)
	void CheckMoveDire();

	// 4�̃G���A�ł̕]��
	void CheckFourAreas();



};

