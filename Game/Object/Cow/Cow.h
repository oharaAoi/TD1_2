﻿#pragma once

#include <Novice.h>
#include <vector>
#include <algorithm>
#include "ImGuiManager.h"

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "MapChip.h"
#include "BaseCow.h"

// ----- Config ----- //
#include "CanMoveDirection.h"

// ----- lib ----- //
#include "MyMath.h"

// ----- Maneger ----- //
#include "Inputs.h"
#include "Turn.h"

enum direEvaluateGrid {
	grid_cow,
	grid_top,
	grid_bottom,
	grid_left,
	grid_right,
	grid_leftTop,
	grid_rightTop,
	grid_leftBottom,
	grid_rightBottom
};


class Cow final
	: public BaseMap,
	public BaseCow,
	public Turn {
private:

	// member objcet

	// ワールド座標での中心点
	Vec2f worldCenterPos_;
	Vec2f worldPreCenterPos_;
	Vec2f size_;
	int gh_;

	Vec2 centerAdd_;

	//==============================================
	// csvを読み込んでその範囲を評価する
	std::vector<std::vector<int>>evaluateGrid_;

	Vec2 localCenterAdd_;

	struct Address {
		Vec2 localAdd;
		Vec2 worldAdd_;
	};

	struct MoveArea {
		Vec2f worldPos;

		QuadVerf screenVertex;

		Matrix3x3 screenMatrix;
		Matrix3x3 worldMatrix;
	};

	Address cannotMove_[8];

	MoveArea moveAreas_[8];

	//==============================================

	// 牛が動くかどうか
	bool isIdle_;

	bool isDisplay_;

	// 各空間での頂点座標
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ワールドとスクリーン空間での行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	//=========================================
	// 移動方向/量
	Vec2f moveDire_;
	Vec2f moveValue_;
	Vec2 moveScalar_;

	// 牛が動く方向の評価値
	int canMoveDireValue_[8];

	int maxDireValue_;
	int maxDireValueIndex_;
	int adjoinNum_;

	int gridDistanceValue_[8];
	int nearWallOfValue_[4];

	//　動いた方向
	kCanMoveDirection movedDire_;

	//=========================================
	// 評価値
	// 壁の距離での評価値
	struct Value {
		int wall;
		int fourArea;
		int allDire;
		int clamp;

		int dog;
		int fence;
		int slantFence;
		int rock;
		int adjoin;
		int adjoinAdd;
	};

	Value value_;

	// 壁との評価で使う
	int wallMin_;
	int wallMinIndex_;
	int wallMinNum_;
	bool wallIsDuplicate_;

	//=========================================
	// フェンスを攻撃するかどうか
	bool isFenceAttack_;

	//=========================================
	

public:
	// Constructor & Destructor
	Cow();
	~Cow();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	//===============================================================
	// user method

	void CheckMoveDire();

	void Move();

	// address
	void CenterAddUpdate();

	// direInit
	void DireInit();

	// 動ける範囲の座標を求める
	void MoveAreasUpdate();

	// それぞれの最短距離から評価値を加算する
	void CheckNearPerson();

	// 牛の位置から1番近い壁を計算する
	void CheckNearWall();

	// スクリーン行列と各頂点の計算
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド行列と各頂点の計算
	void MakeWorldMatrix();

	// imguiの表示
	void ImguiDraw();

	// debugKeys
	void CowMove();

	// debugScreenPrintf
	void DebugScreenPrintf();

	//===============================================================
	// accessor

	// worldPos
	void SetWorldCenterPos(Vec2f pos) { worldCenterPos_ = pos; }
	Vec2f GetWorldCneterPos() { return  worldCenterPos_; }

	// prePos
	void SetPreCenterPos(Vec2f preCenterPos) { worldPreCenterPos_ = preCenterPos; }
	Vec2f GetPreCenterPos() { return worldPreCenterPos_; }

	// address
	Vec2 GetCenterAdd() { return centerAdd_; }

	// 移動できる状態かどうか判断
	void SetIsIdle(bool isIdle) { isIdle_ = isIdle; }
	bool GetIsIdle() { return isIdle_; }

	// 移動した方向を取得
	kCanMoveDirection GetMovedDire() { return movedDire_; }

	// 方向を決めるための量
	void SetMoveDireValue(int value, int num) { canMoveDireValue_[num] = value; }
	int GetMoveDireValue(int num)const { return canMoveDireValue_[num]; }

	// 牛から見たプレイヤー達の位置を計算するために使う
	void SetGridDistanceValue(int value, int dire) { gridDistanceValue_[dire] = value; }
	int GetGridDistanceValue(int dire) { return gridDistanceValue_[dire]; }

	// フェンスを攻撃するかどうかで使う
	void SetIsFenceAttack(bool isAttack) { isFenceAttack_ = isAttack; }
	bool GetIsFenseAttack() { return isFenceAttack_; }

	// 評価値
	int GetAllDireValue() { return value_.allDire; }
	int GetFourAreaValue() { return value_.fourArea; }
	int GetClampValue() { return value_.clamp; }
	int GetDogValue() { return value_.dog; }
	int GetFenceValue() { return value_.fence; }
	int GetSlantFenceValue() { return value_.slantFence; }
	int GetAdjoinValue() { return value_.adjoin; }
	int GetAdjoinAddValue() { return value_.adjoinAdd; }
	int GetRockValue() { return value_.rock; }

	void SetFenceValue(int value) { value_.fence = value; }

	// gridでの評価
	std::vector<std::vector<int>> GetEvaluteGrid() { return evaluateGrid_; }

	Vec2 GetCantMoveAdd(int dire) { return cannotMove_[dire].worldAdd_; }

};

