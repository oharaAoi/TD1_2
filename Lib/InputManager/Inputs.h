#pragma once
#include "InputManager.h"
class Inputs {
private:

	static InputManager* input;

public:
	static void SetInstance();
	static void Update();

	static bool IsPressKey(int DIK) { return input->IsPressKey(DIK); }
	static bool IsTriggerKey(int DIK) { return input->IsTriggerKey(DIK); }
	static bool IsReleaseKey(int DIK) { return input->IsReleaseKey(DIK); }

	static Vec2 GetMousePos() { return input->GetMousePos(); }
	static bool IsPressMouse(int bottomNo) { return input->IsPressMouse(bottomNo); }
	static bool IsTriggerMouse(int bottomNo) { return input->IsTriggerMouse(bottomNo); }
	static bool IsReleaseMouse(int bottomNo) { return input->IsReleaseMouse(bottomNo); }

	static bool IsWheelUP(void) { return input->GetWheel() > 0; }
	static bool IsWheelDown(void) { return input->GetWheel() < 0; }


};

