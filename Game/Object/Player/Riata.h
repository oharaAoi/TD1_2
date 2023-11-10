#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Quad.h"
#include "Matrix3x3.h"
#include "Draw.h"
#include "InputManager.h"

#include <Novice.h>


/// <summary>
/// 投げ縄
/// </summary>
class Riata {
private: // member object

	InputManager* input = InputManager::GetInstance();

	Vec2f worldPos_;
	Vec2f size_;
	int gh_;
	unsigned int color_;

	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	bool isIdle_;
	bool isStart_;

public: // member method
	// Constructor & Destructor
	Riata();
	~Riata();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	// user method
	void MakeWorldMatrix();

	void MakeScreenMatrix(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

private:


};