#pragma once

#include <vector>
using namespace std;

enum kTurnType {
	Players,
	Cows
};

class Turn {
protected:

	static int turnNo_;
	static int preTurnNo_;
	static bool isTurnChange_;
	static kTurnType turnType_;
	bool IsTurnChange();

	static int movingCountLimit_;
	static int movingCount_;

public:
	virtual ~Turn();

	static void Init();
	static void TurnNoUpdate();

	static void SetMovingCountLimit(int movingCountLimit) { movingCountLimit_ = movingCountLimit; };
	static int GetMovingCountLimit() { return movingCountLimit_; };

	static void SetMovingCount(int movingCount) { movingCount_ = movingCount; };
	static int GetMovingCount() { return movingCount_; };

};

