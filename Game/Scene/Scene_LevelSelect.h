#pragma once

#include <Novice.h>

// ----- base ----- //
#include "BaseScene.h"


class Scene_LevelSelect : public BaseScene {
private:
	// member object



public:
	// Constructor & Destructor
	Scene_LevelSelect();
	~Scene_LevelSelect();

	// default method
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

};

