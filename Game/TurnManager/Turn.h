#pragma once

class Turn {
protected:

	static int movingCount_;
	static int turnNo_;
	static int preTurnNo_;
	static bool isTurnChange_;
	bool IsTurnChange();

public:
	virtual ~Turn();

	static void TurnNoUpdate();
};

