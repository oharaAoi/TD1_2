#include "Inputs.h"

InputManager* Inputs::input = nullptr;

void Inputs::SetInstance() {
	input = InputManager::GetInstance();
}


void Inputs::Update() {
	input->Update();
}
