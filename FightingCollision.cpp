#include "FightingCollision.h"

FightingCollision::FightingCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog){
	Init(cowherd, youngPerson, mapChip, fighting, dog);
}

FightingCollision::~FightingCollision() { Finalize(); }

void FightingCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

/*========================================================
	�������֐�
========================================================*/
void FightingCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullFighting* fighting, Dog* dog){
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	fighting_ = fighting;
	dog_ = dog;
}


/*========================================================
	�X�V�����֐�
========================================================*/


/*========================================================
	�]��
========================================================*/


/*==========================================================================================================
										��Ƃ̕]��
============================================================================================================*/
void FightingCollision::CheckRockAdjoin(){
}

/*==========================================================================================================
										�i�s�����̕]��(csv�ł��)
============================================================================================================*/
void FightingCollision::CheckMoveDire(){
}

/*==========================================================================================================
										4�̃G���A�ł̕]��
============================================================================================================*/
void FightingCollision::CheckFourAreas(){
}