#include "BullCollision.h"

BullCollision::BullCollision(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog){
	Init(cowherd, youngPerson, mapChip, bullCow, dog);
}

BullCollision::~BullCollision(){
}

void BullCollision::Finalize() {
	cowherd_ = nullptr;
	youngPerson_ = nullptr;
}

void BullCollision::Init(Cowherd* cowherd, YoungPerson* youngPerson, MapChip* mapChip, BullCow* bullCow, Dog* dog) {
	cowherd_ = cowherd;
	youngPerson_ = youngPerson;
	mapChip_ = mapChip;
	bullCow_ = bullCow;
	dog_ = dog;
}

void BullCollision::CheckBullMoveDire(){

	// 犬がいる時
	CheckDogExist();

	// 両隣フェンスの時
	CheckCowAdjoin();
	/*CheckCowFourArea();*/
	// マス目の計算(牧師
	//CheckGridDistance(cowherd_->GetCenterAdd());

	//// マス目の計算(若人
	//for (int i = 0; i < youngPerson_->GetYoungMaxIndex(); i++) {
	//	CheckGridDistance(youngPerson_->GetCenterAdd(i));
	//}

	// 4つのエリア
	CheckFourAreas();

	// 牛から見た全方向
	CheckCowMoveAllDire();

	// 牛がフェンスと隣の時
	CheckFenseCollision();

}

void BullCollision::CheckCowAdjoin(){
}

void BullCollision::CheckDogExist(){
}

void BullCollision::CheckCowFourArea(){
}

//void BullCollision::CheckGridDistance(const Vec2& add){
//}

void BullCollision::CheckCowMoveAllDire(){
}

void BullCollision::CheckFourAreas(){
}

//void BullCollision::CheckGridDire(const Vec2& add){
//}

void BullCollision::CheckFenseCollision(){
}
