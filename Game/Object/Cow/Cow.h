#pragma once

#include <Novice.h>
#include <vector>
#include <algorithm>

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "MapChip.h"

// ----- Config ----- //
#include "CanMoveDirection.h"

class Cow : public BaseMap {
private:
	// member objcet

	// ワールド座標での中心点
	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	Vec2 centerAdd_;

	// 牛が動くかどうか
	bool isIdle_;

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

	// 牛が動く方向の評価値
	int canMoveDireValue_[8];
	int gridDistanceValue_[8];
	int nearWallOfValue_[4];
	
	//=========================================
	// 評価値
	// 壁の距離での評価値
	struct Value {
		int wall;
	};
	
	Value value_;

	// 壁との評価で使う
	int wallMin_;
	int wallMinIndex_;
	int wallMinNum_;
	bool wallIsDuplicate_;

public:
	// Constructor & Destructor
	Cow(MapChip* mapChip);
	~Cow()override;

	// default method
	void Init(MapChip* mapChip);
	void Update();
	void Draw();
	void Finalize();

	// user method

	// address
	void CenterAddUpdate();

	// direInit
	void DireInit();

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


	// accessor

	// address
	Vec2 GetCenterAdd() { return centerAdd_; }

	// 方向を決めるための量
	void SetMoveDireValue(int value, int num) { canMoveDireValue_[num] = value; }
	int GetMoveDireValue(int num) { return canMoveDireValue_[num]; }

	// 牛から見たプレイヤー達の位置を計算するために使う
	void SetGridDistanceValue(int value, int dire) {gridDistanceValue_[dire] = value; }
	int GetGridDistanceValue(int dire) { return gridDistanceValue_[dire]; }

};

