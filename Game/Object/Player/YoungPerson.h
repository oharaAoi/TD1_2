#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "InputManager.h"

// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"

class YoungPerson final
	: public BaseMap {
private:

	// member object
	InputManager* input = InputManager::GetInstance();

	// member object

	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	bool isMoveIdle_;

	// 移動できる方向
	enum kCanMoveDirection {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	// 移動方向/移動量
	Vec2f moveDir_;
	Vec2f moveValue_;

	bool canMoveDir_[4];

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
	void Move();

	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	void MakeWorldMatrix();



	// accessor

	void SetCanMoveDir(bool canMove, kCanMoveDirection dir) { canMoveDir_[dir] = canMove; }

};

