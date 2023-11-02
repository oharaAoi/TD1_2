#pragma once

#include <Novice.h>
#include <vector>

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"

class Cow final{
private:
	// member objcet

	// ワールド座標での中心点
	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	// 各空間での頂点座標
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ワールドとスクリーン空間での行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

public:
	// Constructor & Destructor
	Cow(Vec2f cowPos);
	~Cow();

	// default method
	void Init(Vec2f cowPos);
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

