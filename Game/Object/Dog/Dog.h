#pragma once

#include <Novice.h>

#include "InputManager.h"

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "Collision.h"

// ----- object ----- //
#include "BaseMap.h"

class Dog final :public BaseMap {
private: 

	// 入力処理のインスタンス確保
	InputManager* input = InputManager::GetInstance();

	// member object

	// ワールド空間での中心座標
	Vec2f worldCenterPos_;
	Vec2f size_;
	int gh_;

	// 犬を置くかどうか
	bool isIdle_;

	// 犬がおいてあるかのフラグ
	bool isExist_;

	// 犬がどこにおいてあるか
	bool isExistSide_[4];

	// 各空間の頂点座標
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;
	
	// ローカル空間以外の各行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;


	//=================================================
	//　犬を置ける場所
	struct PutPlace {
		Vec2f worldPos;

		Vec2 localAdd;
		Vec2 worldAdd;

		QuadVerf worldVertex;
		QuadVerf screenVertex;

		Matrix3x3 worldMatrix;
		Matrix3x3 screenMatrix;
	};

	PutPlace putPlace_[4];

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

	// 犬を設置するために必要
	void Install();

	// 犬を置く
	void Put();

	// アドレスの計算
	Vec2 CalcCenterAdd(Vec2f centerPos);

	// スクリーン空間に変更するための関数
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド空間の行列と各頂点の計算
	void MakeWorldMatrix();

	// デバック用
	void DebugScreen();

	// accessor

	void SetIsIdle(bool isIdle) { isIdle_ = isIdle; }
	bool GetISIdle() { return isIdle_; }

	bool GetIsExist() { return isExist_; }

	bool GetIsExistSide(int num) { return isExistSide_[num]; }
};

