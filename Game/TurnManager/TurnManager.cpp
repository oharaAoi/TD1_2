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
	
	movingCount_ = 0;

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
	isMove_ = false;

	if (cowherd_->GetIsMove()) {
		isMove_ = true;
	}

	for (int yi = 0; yi < young_->GetYoungMaxIndex(); yi++) {
		if (young_->GetIsMove(yi)) {
			isMove_ = true;
		}
	}
	
	if (isMove_ && !preIsMove_) {
		movingCount_++;
		if (movingCount_ >= 2) {
			isTurnChange_ = true;
			movingCount_ = 0;
		}
	}


	if (isTurnChange_) {
		turnNo_++;
		isTurnChange_ = false;
	}

}


/*================================================================
	描画処理関数
================================================================*/
void TurnManager::Draw() {

	Novice::ScreenPrintf(100, 200, "movingCount = %d", movingCount_);
	Novice::ScreenPrintf(100, 220, "turnNo = %d", turnNo_);

}


/*================================================================
	終了処理関数
================================================================*/
void TurnManager::Finalize() {

	young_ = nullptr;
	cowherd_ = nullptr;
}
