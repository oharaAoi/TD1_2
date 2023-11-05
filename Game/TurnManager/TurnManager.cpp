#include "TurnManager.h"

TurnManager::TurnManager(YoungPerson* young, Cowherd* cowherd) {
	Init(young, cowherd);
}

TurnManager::~TurnManager() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void TurnManager::Init(YoungPerson* young, Cowherd* cowherd) {

	young_ = young;
	cowherd_ = cowherd;

	turnNo_ = 0;
	isTurnChange_ = false;

	isMove_ = false;
	preIsMove_ = false;

}


/*================================================================
	更新処理関数
================================================================*/
void TurnManager::Update() {
	preIsMove_ = isMove_;

	
	if (isMove_ && !preIsMove_) { isTurnChange_ = true; }

	if (isTurnChange_) {
		turnNo_++;
		isTurnChange_ = false;
	}

}


/*================================================================
	描画処理関数
================================================================*/
void TurnManager::Draw() {


}


/*================================================================
	終了処理関数
================================================================*/
void TurnManager::Finalize() {

	young_ = nullptr;
	cowherd_ = nullptr;
}
