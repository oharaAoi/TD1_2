#include "Turn.h"

int Turn::turnNo_ = 0;
int Turn::preTurnNo_ = 0;
bool Turn::isTurnChange_ = false;
kTurnType Turn::turnType_ = kTurnType::Players;

int Turn::movingCountLimit_ = 0;
int Turn::movingCount_ = 0;

Turn::~Turn() {}

void Turn::Init() {
	turnNo_ = 0;
	preTurnNo_ = 0;
	isTurnChange_ = 0;
	turnType_ = kTurnType::Players;
	
	movingCountLimit_ = 0;
	movingCount_ = 0;

}

void Turn::TurnNoUpdate() {
	preTurnNo_ = turnNo_;
}

bool Turn::IsTurnChange() {
	return turnNo_ != preTurnNo_;
}
