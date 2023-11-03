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

	int youngIndex;

	struct Base {
		// world座標での矩形の中心
		Vec2f worldCenterPos;
		Vec2f keepCenterPos;

		// 各頂点座標
		QuadVerf worldVertex;
		QuadVerf screenVertex;

		// 各座標系の行列
		Matrix3x3 worldMatrix;
		Matrix3x3 screenMatrix;

		// 移動待機状態のフラグ
		bool isMoveIdle;

		// 移動方向/移動量
		Vec2f moveDir;
		Vec2f moveValue;

		bool canMoveDir[4];
	};

	std::vector<Base> young_;

	Vec2f size_;
	int gh_;

	QuadVerf localVertex_;

	// 移動できる方向
	enum kCanMoveDirection {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};



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

	void SetCanMoveDir(bool canMove, kCanMoveDirection dir, int num) { 
		young_[num].canMoveDir[dir] = canMove;
	}

};

