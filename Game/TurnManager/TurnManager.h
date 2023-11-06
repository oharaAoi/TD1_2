#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"

class TurnManager {
private:

	// プレイヤーへのポインタ
	YoungPerson* young_;
	Cowherd* cowherd_;

	int turnNo_;
	bool isTurnChange_;

	int movingCount_;
	
	bool isMove_;
	bool preIsMove_;

public:
	// Constructor & Destuctor
	TurnManager(YoungPerson* young, Cowherd* cowherd);
	~TurnManager();

	// default method
	void Init(YoungPerson* young, Cowherd* cowherd);
	void Update();
	void Draw();
	void Finalize();

};

