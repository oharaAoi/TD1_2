#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "InputManager.h"
#include "Collision.h"
#include "Ease.h"


// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"

class YoungPerson final
	: public BaseMap {

private:

	enum kGridType {
		None,
		Player,
		CanMove
	};

	struct CanMoveGrid final {

		Vec2f worldCenterPos;
		Vec2 localAdd;
		Vec2 worldAdd;

		QuadVerf localVertex;
		QuadVerf screenVertex;

		Matrix3x3 worldMatrix;
		Matrix3x3 screenMatrix;

		bool canMove;
	};

	struct Base final {
		// world座標での矩形の中心
		Vec2f worldCenterPos;
		Vec2f destinationPos;
		Vec2f startingPos;
		
		Vec2f scale;

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
		Vec2 localCenterAdd;
		std::vector<CanMoveGrid> canMoveGrid;

		bool isMove;
	};

private:

	// member object
	InputManager* input = InputManager::GetInstance();

	// member object

	int maxYoungIndex_;

	std::vector<Base> young_;

	Vec2f size_;
	int gh_;

	QuadVerf localVertex_;

	// ----- 移動に関するもの ----- //

	std::vector<std::vector<int>> moveGrid_;

	int moveGridMaxIndex_;
	float movingTime_;

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


	// centerAdd
	Vec2 GetCenterAdd(int index) const { return young_[index].centerAdd; }

	// indexMax
	/*int GetIndexMax()const { return indexMax; }*/

	// isMoveIdle
	bool GetIsMoveIdle(int index) const { return young_[index].isMoveIdle; }

	// canMoveGrid
	std::vector<CanMoveGrid> GetCanMoveGrid(int index) const { return young_[index].canMoveGrid; }

	int GetYoungMaxIndex() const { return maxYoungIndex_; }

	int GetCanMoveGridMaxIndex() const { return moveGridMaxIndex_; }

	void SetCanMove(bool canMove, int yi, int gi) { young_[yi].canMoveGrid[gi].canMove = canMove; }

};

