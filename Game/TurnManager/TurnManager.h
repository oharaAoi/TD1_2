#pragma once

// ----- object ----- //
#include "Cowherd.h"
#include "YoungPerson.h"
#include "Turn.h"

class TurnManager 
	: public Turn {
private:
public:
	// Constructor & Destuctor
	TurnManager();
	~TurnManager();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

};

