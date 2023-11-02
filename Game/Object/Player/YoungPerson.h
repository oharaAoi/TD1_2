#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"

class YoungPerson final {
private:
	// member object

	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;


public:
	// Constructor & Destructor
	YoungPerson();
	~YoungPerson();


	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	// user method

	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	void MakeWorldMatrix();



	// accessor

};

