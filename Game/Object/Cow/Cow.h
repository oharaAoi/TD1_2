#pragma once

#include <Novice.h>
#include <vector>

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "MapChip.h"

// ----- Config ----- //
#include "CanMoveDirection.h"

class Cow final{
private:
	// member objcet

	// ワールド座標での中心点
	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	// 移動方向/量
	Vec2f moveDire_;
	Vec2f moveValue_;

	// 牛が動く方向
	bool canMoveDie_[8];

	// 各空間での頂点座標
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ワールドとスクリーン空間での行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

public:
	// Constructor & Destructor
	Cow(MapChip* mapChip);
	~Cow();

	// default method
	void Init(MapChip* mapChip);
	void Update();
	void Draw();
	void Finalize();

	// user method

	// スクリーン行列と各頂点の計算
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド行列と各頂点の計算
	void MakeWorldMatrix();


	// accessor

};

