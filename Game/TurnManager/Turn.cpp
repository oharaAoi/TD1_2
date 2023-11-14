#include "Turn.h"

int Turn::turnNo_ = 0;
int Turn::preTurnNo_ = 0;
bool Turn::isTurnChange_ = false;
kTurnType Turn::turnType_ = kTurnType::Players;

int Turn::yP_maxIndex_ = 0;
vector<bool> Turn::yP_isMove_ = static_cast<vector<bool>>(false);
bool Turn::ch_isMove_ = false;

Turn::~Turn() {}

void Turn::Init() {
	turnNo_ = 0;
	preTurnNo_ = 0;
	isTurnChange_ = 0;
	turnType_ = kTurnType::Players;
	
	for (int yi = 0; yi < yP_maxIndex_; yi++) {
		yP_isMove_[yi] = false;
	}
	ch_isMove_ = false;

}

void Turn::TurnNoUpdate() {
	preTurnNo_ = turnNo_;
}

bool Turn::IsTurnChange() {
	return turnNo_ != preTurnNo_;
}
