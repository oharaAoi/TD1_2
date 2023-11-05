#pragma once
#include "BaseScene.h"

// ----- math ----- //
#include "InputManager.h"
#include <SafeDelete.h>

class Scene_Clear : public BaseScene{

private:

	// ���͏����̃C���X�^���X�m��
	InputManager* input = InputManager::GetInstance();

public:

	Scene_Clear();
	~Scene_Clear();

	// �f�t�H���g�֐��̃I�[�o�[���C�h
	void Init() override; // ������
	void Update() override; // �X�V
	void Draw() override; // �`��
	void Finalize() override; // �I��
	

};

