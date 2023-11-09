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

	if (input->IsTriggerKey(DIK_SPACE)) {
		turnNo_++;
	}

	if (ch_isMove_) {
		for (int yi = 0; yi < yP_maxIndex_; yi++) {
			if (!yP_isMove_[yi]) {
				break;
			}

			if (yi >= yP_maxIndex_ - 1) {
				turnNo_++;
			}
		}
	}

	isTurnChange_ = IsTurnChange();

	if (isTurnChange_) {
		turnType_ = kTurnType::Cows;

		// 移動フラグの初期化
		ch_isMove_ = false;
		for (int yi = 0; yi < yP_maxIndex_; yi++) {
			yP_isMove_[yi] = false;
		}
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
