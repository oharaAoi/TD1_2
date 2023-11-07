#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "InputManager.h"
#include "Drawable.h"

enum kCowType {
	Calf,
	Bull,
	Fighting
};

/// <summary>
/// 牛の基底クラス
/// </summary>
class BaseCow
	: public Drawable {
protected: // member object

	// 入力処理; 後で消す
	InputManager* input = InputManager::GetInstance();

	// 牛の種類; いるかわからない
	kCowType type_;

	// 絶対必要なメンバ変数 -----------------------

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

	// ----- 移動に使う変数を追加する ----- //


	// ----- 評価クラスを追加する ----- //


	// -----------------------------------------

public:
	// 仮想デストラクタ
	virtual ~BaseCow();

	// default method
	virtual void Init() = 0;
	virtual void Update() = 0;
	void Draw() override;
	virtual void Finalize() = 0;

};

