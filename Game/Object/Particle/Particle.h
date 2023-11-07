#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Color.h"

class Particle {
private:

	// member object
	Vec2f pos;
	Vec2f velocity;
	Vec2f acceleration;
	float radius;
	float lifeTime;
	float currentTime;
	Color color;
	float raito;
	float max;
	float boundPoint;

public:
	// Constructor & Destructor
	Particle();
	~Particle();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

};

