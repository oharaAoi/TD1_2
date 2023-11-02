#pragma once

#include <Novice.h>

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"

class Dog {
private:
	// member object

	// ワールド空間での中心座標
	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	// 各空間の頂点座標
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;
	
	// ローカル空間以外の各行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;


public:
	// Constructor & Destructor
	Dog();
	~Dog();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	// user method

	// スクリーン空間に変更するための関数
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド空間の行列と各頂点の計算
	void MakeWorldMatrix();

	// accessor


};

