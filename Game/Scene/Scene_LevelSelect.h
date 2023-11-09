#pragma once

#include <Novice.h>

// ----- base ----- //
#include "BaseScene.h"

// ----- math ----- //
#include "Inputs.h"

class Scene_LevelSelect
	: public BaseScene {
private:
	// member object

	static int selectStage_;


public:
	// Constructor & Destructor
	Scene_LevelSelect();
	~Scene_LevelSelect();

	// default method
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	static int GetSelectStage() { return selectStage_; }

};

