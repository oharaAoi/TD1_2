#include "TurnManager.h"

TurnManager::TurnManager() {
	Init();
}

TurnManager::~TurnManager() { Finalize(); }


/*================================================================
	初期化関数
================================================================*/
void TurnManager::Init() {

	turnNo_ = 0;
	isTurnChange_ = false;

}


/*================================================================
	更新処理関数
================================================================*/
void TurnManager::Update() {
	TurnNoUpdate();

	if (Inputs::IsTriggerKey(DIK_M)) {
		turnNo_++;
	}

	if (movingCount_ >= movingCountLimit_) {
		turnNo_++;
	
	}

	isTurnChange_ = IsTurnChange();

	if (isTurnChange_) {
		turnType_ = kTurnType::Cows;

		// 動いた回数のリセット
		movingCount_ = 0;
	}
	/*else {
		turnType_ = kTurnType::Players;
	}*/

}


/*================================================================
	描画処理関数
================================================================*/
void TurnManager::Draw() {

	Novice::ScreenPrintf(100, 220, "turnNo = %d", turnNo_);
	Novice::ScreenPrintf(100, 240, "isTurnChange_ = %d", isTurnChange_);

}


/*================================================================
	終了処理関数
================================================================*/
void TurnManager::Finalize() {}
