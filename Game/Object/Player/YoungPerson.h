﻿#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "Inputs.h"
#include "Collision.h"
#include "Ease.h"
#include "Drawable.h"

// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"
#include "Turn.h"


#include "Stack.h"


class YoungPerson final
	: public BaseMap,
	public Drawable,
	public Turn {
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
		bool isMoved;
		// スタン
		bool isStan;

		// 吹き飛ばされるフラグ
		bool isStriked;
	};

private:


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
	// 全体での移動待機フラグ
	bool isMoveIdle_;

public:
	/* ---------- Constructor & Destructor ---------- */
	YoungPerson();
	~YoungPerson();


	/* ---------- default method ---------- */
	void Init();
	void Update();
	void Draw() override;
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


public: /* ---------- accessor ---------- */

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

	bool GetIsMove(int yi) const { return young_[yi].isMove; }

	void SetWorldPos(const Vec2f& pos, int yi) { young_[yi].worldCenterPos = pos; }
	Vec2f GetWorldPos(int yi) { return young_[yi].worldCenterPos; }

	void SetIsMove(bool isMove, int yi) { young_[yi].isMove = isMove; }

	void SetIsMoved(bool isMoved, int yi) { young_[yi].isMoved = isMoved; }

	void SetYoung_IsMoveIdle(bool isMoveIdle, int yi) { young_[yi].isMoveIdle = isMoveIdle; }
	void SetIsMoveIdle(bool isMoveIdle) { isMoveIdle_ = isMoveIdle; }
	//void SetYP_IsMove(bool yP_isMove, int yi) { yP_isMove_[yi] = yP_isMove; }

	//bool GetYP_IsMove(int yi) { return yP_isMove_[yi]; }

	// スタン
	void SetIsStan(bool isStan, int yi) { young_[yi].isStan = isStan; }
	bool GetIsStan(int yi) { return young_[yi].isStan; }

	// ぶっとばされる
	void SetIsStriked(bool isStriked, int yi){ young_[yi].isStriked = isStriked; }
	bool GetIsStriked(int yi) { return young_[yi].isStriked; }

};

