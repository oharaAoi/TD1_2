#include "Turn.h"

int Turn::movingCount_ = 0;
int Turn::turnNo_ = 0;
int Turn::preTurnNo_ = 0;
bool Turn::isTurnChange_ = false;

Turn::~Turn() {}

void Turn::TurnNoUpdate() {
	preTurnNo_ = turnNo_;
}

bool Turn::IsTurnChange() {
	return turnNo_ != preTurnNo_;
}
