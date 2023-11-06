#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"

enum kCowType {
	Calf,
	Bull,
	Fighting
};

/// <summary>
/// 牛の基底クラス
/// </summary>
class BaseCow {
protected:

	kCowType type_;

	Vec2f worldPos_;
	Vec2f worldAdd_;
	Vec2f size_;
	int gh_;

	// 各空間の頂点
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ローカル以外の各行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

public:
	virtual ~BaseCow();



};

