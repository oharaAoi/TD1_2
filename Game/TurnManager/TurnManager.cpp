#include "TurnManager.h"

TurnManager::TurnManager() {
	Init();
}

TurnManager::~TurnManager() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void TurnManager::Init() {

	movingCount_ = 0;

	turnNo_ = 0;
	isTurnChange_ = false;

}


/*================================================================
	更新処理関数
================================================================*/
void TurnManager::Update() {
	TurnNoUpdate();

	if (movingCount_ >= 2) {
		turnNo_++;
		movingCount_ = 0;
	}

	isTurnChange_ = IsTurnChange();

}


/*================================================================
	描画処理関数
================================================================*/
void TurnManager::Draw() {

	Novice::ScreenPrintf(100, 200, "movingCount = %d", movingCount_);
	Novice::ScreenPrintf(100, 220, "turnNo = %d", turnNo_);
	Novice::ScreenPrintf(100, 240, "isTurnChange_ = %d", isTurnChange_);

}


/*================================================================
	終了処理関数
================================================================*/
void TurnManager::Finalize() {}
