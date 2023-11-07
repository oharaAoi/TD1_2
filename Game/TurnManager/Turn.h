#pragma once

enum kTurnType {
	Players,
	Cows
};

class Turn {
protected:

	static int movingCount_;
	static int turnNo_;
	static int preTurnNo_;
	static bool isTurnChange_;
	static kTurnType turnType_;
	bool IsTurnChange();

public:
	virtual ~Turn();

	static void TurnNoUpdate();
};

