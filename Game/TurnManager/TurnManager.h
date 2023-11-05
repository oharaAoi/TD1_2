#pragma once


class TurnManager {
private:

	int turnNo_;
	bool isTurnChange_;

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

