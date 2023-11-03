#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "InputManager.h"
#include "Collision.h"
#include "CanMoveDirection.h"

// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"


/// <summary>
/// 牛飼いクラス
/// </summary>
class Cowherd
	: public BaseMap {
private:

	// member object
	InputManager* input = InputManager::GetInstance();

	// ワールド空間での中心点
	Vec2f worldCenterPos_;
	Vec2 centerAdd_;
	Vec2f size_;
	int gh_;

	// 各空間の頂点
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ローカル以外の各空間の行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	bool isMoveIdle_;

	// 移動方向と移動量
	Vec2f moveDir_;
	Vec2f moveValue_; // 初期化の値を変更で移動量を変化させる
	bool canMoveDir_[8];

private: // 移動に関するもの

	enum kGridType {
		None,
		Player,
		CanMove
	};

	std::vector<std::vector<int>> moveGrid_;
	Vec2 localCenterAdd_;


	struct CanMoveGrid final {

		Vec2f worldCenterPos;
		Vec2 localAdd;
		Vec2 worldAdd;

		QuadVerf localVertex;
		QuadVerf screenVertex;

		Matrix3x3 worldMatrix;
		Matrix3x3 screenMatrix;

	};

	int maxIndex_;
	std::vector<CanMoveGrid> canMoveGrid_;


public:
	// Constructor & Destructor
	Cowherd();
	~Cowherd();

	// default method
	void Init();
	void Update();
	void Draw();
	void Finalize();

	// user method

	// ワールド空間行列をスクリーン空間行列に変換する関数
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	void MakeWorldMatrix();

	void Move();

	void DebugDraw();

	// 中心点のアドレスを計算
	void CenterAddUpdate();

	// accessor

	// canMoveDir
	void SetCanMoveDir(bool canMove, kCanMoveDirection canMoveDir) { canMoveDir_[canMoveDir] = canMove; }

	// centerAdd
	Vec2 GetCenterAdd() const { return centerAdd_; }

	// isMoveIdle
	bool GetIsMoveIdle() const { return isMoveIdle_; }

};

