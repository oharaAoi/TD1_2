#include "Camera.h"
#include <Novice.h>

#include "MyMath.h"
#include "Environment.h"

Camera::Camera() { Init(); }

Camera::~Camera() {}

void Camera::Init() {

	// member object
	// マップチップ用に少しずらした
	center_ = { 320.0f, 320.0f };

	top_ = 360.0f;
	bottom_ = -360.0f;
	left_ = -640.0f;
	right_ = 640.0f;
	scale_ = 1.0f;

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, center_);
	viewMatrix_ = MakeInverseMatrix(worldMatrix_);
	orthoMatrix_ = MakeOrthoMatrix(top_, bottom_, left_, right_);
	viewportatrix_ = MakeViewportMatrix(0, 0, static_cast<float>(kWindowSize.x), static_cast<float>(kWindowSize.y));


	vorticalStart_ = Transform({ 0.0, 0.0f }, MakeWvpVpMatrix(viewMatrix_, worldMatrix_, orthoMatrix_, viewportatrix_));
	vorticalEnd_ = Transform({ 0.0, 720.0f }, MakeWvpVpMatrix(viewMatrix_, worldMatrix_, orthoMatrix_, viewportatrix_));

	horizontalStart_ = Transform({ 0.0f, 0.0f }, MakeWvpVpMatrix(viewMatrix_, worldMatrix_, orthoMatrix_, viewportatrix_));
	horizontalEnd_ = Transform({ 1280.0f, 0.f }, MakeWvpVpMatrix(viewMatrix_, worldMatrix_, orthoMatrix_, viewportatrix_));

}

void Camera::Update() {

	if (Inputs::IsWheelUP()) {
		scale_ += 0.1f;
	}

	if (Inputs::IsWheelDown()) {
		if (scale_ > 1.0f) {
			scale_ -= 0.1f;
		}
	}

	worldMatrix_ = MakeAffineMatrix({ 1.0f,1.0f }, 0.0f, center_);
	viewMatrix_ = MakeInverseMatrix(worldMatrix_);
	orthoMatrix_ = MakeOrthoMatrix(top_* scale_, bottom_ * scale_, left_ * scale_, right_ * scale_);
	viewportatrix_ = MakeViewportMatrix(0, 0, static_cast<float>(kWindowSize.x), static_cast<float>(kWindowSize.y));


}

void Camera::Draw() {


	/*Novice::DrawLine(
		static_cast<int>(vorticalStart_.x),
		static_cast<int>(vorticalStart_.y),
		static_cast<int>(vorticalEnd_.x),
		static_cast<int>(vorticalEnd_.y),
		0x800000ff
	);

	Novice::DrawLine(
		static_cast<int>(horizontalStart_.x),
		static_cast<int>(horizontalStart_.y),
		static_cast<int>(horizontalEnd_.x),
		static_cast<int>(horizontalEnd_.y),
		0x008000ff
	);*/



}
