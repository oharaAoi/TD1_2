#pragma once

#include "MyVector2.h"
#include "Matrix3x3.h"
#include "InputManager.h"

class Camera {
private:

	InputManager* input = InputManager::GetInstance();

	// member object
	Vec2f center_;
	Vec2f size_;

	float top_;
	float bottom_;
	float left_;
	float right_;
	float scale_;

	Matrix3x3 worldMatrix_;
	Matrix3x3 viewMatrix_;
	Matrix3x3 orthoMatrix_;
	Matrix3x3 viewportatrix_;

	Vec2f vorticalStart_;
	Vec2f vorticalEnd_;
	
	Vec2f horizontalStart_;
	Vec2f horizontalEnd_;

public:
	// constructor & destructor
	Camera();
	~Camera();

	// default method
	void Init();
	void Update();
	void Draw();

	// user method

	// accessor
	Matrix3x3 GetViewMatrix(void) { return viewMatrix_; }
	Matrix3x3 GetOrthoMatrix(void) { return orthoMatrix_; }
	Matrix3x3 GetViewportMatrix(void) { return viewportatrix_; }

	void SetCenter(const Vec2f& center) { center_ = center; }

};