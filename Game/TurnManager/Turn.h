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

	static int yP_maxIndex_;
	static vector<bool> yP_isMove_; // 若人
	static bool ch_isMove_; // 牛飼い

public:
	virtual ~Turn();

	static void Init();
	static void TurnNoUpdate();
};

