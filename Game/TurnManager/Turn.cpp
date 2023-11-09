#include "Turn.h"

int Turn::turnNo_ = 0;
int Turn::preTurnNo_ = 0;
bool Turn::isTurnChange_ = false;
kTurnType Turn::turnType_ = kTurnType::Players;

int Turn::yP_maxIndex_ = 0;
vector<bool> Turn::yP_isMove_ = static_cast<vector<bool>>(false);
bool Turn::ch_isMove_ = false;

Turn::~Turn() {}

void Turn::TurnNoUpdate() {
	preTurnNo_ = turnNo_;
}

bool Turn::IsTurnChange() {
	return turnNo_ != preTurnNo_;
}
