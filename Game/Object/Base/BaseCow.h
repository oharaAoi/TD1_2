#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "InputManager.h"
#include "Drawable.h"

// ----- config ----- //
#include "CanMoveDirection.h"


/// <summary>
/// 牛の基底クラス
/// </summary>
class BaseCow
	: public Drawable {
protected: // member object

	// 入力処理; 後で消す
	InputManager* input = InputManager::GetInstance();

	// 絶対必要なメンバ変数 -----------------------

	Vec2f worldPos_;
	Vec2 worldAdd_;
	Vec2f size_;
	int gh_;

	bool isIdle_;

	// 各空間の頂点
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ローカル以外の各行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	// ----- 移動に使う変数を追加する ----- //
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

	// ----- 評価クラスを追加する ----- //
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
	};

	Value value_;

	// 壁との評価で使う
	int wallMin_;
	int wallMinIndex_;
	int wallMinNum_;
	bool wallIsDuplicate_;

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

