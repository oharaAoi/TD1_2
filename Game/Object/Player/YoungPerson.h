#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "InputManager.h"
#include "CanMoveDirection.h"
#include "Collision.h"

// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"

class YoungPerson final
	: public BaseMap {
private:

	// member object
	InputManager* input = InputManager::GetInstance();

	// member object

	int indexMax;

	struct Address {
		Vec2f worldCenterPos;

		Vec2 add;
		QuadVerf worldVertex;
		QuadVerf screenVertex;

		Matrix3x3 worldMatrix;
		Matrix3x3 screenMatrix;
	};

	struct Base {
		// world座標での矩形の中心
		Vec2f worldCenterPos;
		Vec2f keepCenterPos;

		// youngPersonのいるアドレス
		Vec2 centerAdd;

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

		Address moveDirAdd[4];

		bool canMoveDir[4];
		bool isMove;
	};

	std::vector<Base> young_;

	Vec2f size_;
	int gh_;

	QuadVerf localVertex_;

public:
	/* ---------- Constructor & Destructor ---------- */
	YoungPerson();
	~YoungPerson();


	/* ---------- default method ---------- */
	void Init();
	void Update();
	void Draw();
	void Finalize();

	/* ---------- user method ---------- */
	void Move();

	void CenterAddUpDate();

	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	void MakeWorldMatrix();


	void DebugDraw();


	/* ---------- accessor ---------- */

	// canMoveDir
	void SetCanMoveDir(bool canMove, kCanMoveDirection dir, int num) {
		young_[num].canMoveDir[dir] = canMove;
	}

	// centerAdd
	Vec2 GetCenterAdd(int index) const { return young_[index].centerAdd; }

	// indexMax
	int GetIndexMax()const { return indexMax; }

	// isMoveIdle
	bool GetIsMoveIdle(int index) const { return young_[index].isMoveIdle; }

	Address GetMoveDirAdd(int index, kCanMoveDirection dir) const { return young_[index].moveDirAdd[dir]; }

};

