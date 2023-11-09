#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "Turn.h"
#include "InputManager.h"

class TurnManager 
	: public Turn {
private:

	InputManager* input = InputManager::GetInstance();

public:
	// Constructor & Destuctor
	TurnManager();
	~TurnManager();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	bool GetIsTurnChange(void) const { return isTurnChange_; }

};

