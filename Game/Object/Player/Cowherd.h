#pragma once

// ----- math ----- //
#include "MyVector2.h"
#include "Matrix3x3.h"
#include "Quad.h"
#include "Draw.h"
#include "Inputs.h"
#include "Collision.h"
#include "Ease.h"
#include "MyMath.h"
#include "Drawable.h"
#include <SafeDelete.h>

// ----- object ----- //
#include "MapChip.h"
#include "BaseMap.h"
#include "Turn.h"
#include "Riata.h"
#include "Stack.h"

/// <summary>
/// 牛飼いクラス
/// </summary>
class Cowherd
	: public BaseMap,
	public Drawable,
	public Turn {
private:

	// ワールド空間での中心点
	Vec2f worldCenterPos_;
	Vec2f destinationPos_;
	Vec2f startingPos_;
	Vec2 centerAdd_;
	Vec2f size_;
	Vec2f scale_;
	int white1x1GH_;

	// 各空間の頂点
	QuadVerf localVertex_;
	QuadVerf worldVertex_;
	QuadVerf screenVertex_;

	// ローカル以外の各空間の行列
	Matrix3x3 worldMatrix_;
	Matrix3x3 screenMatrix_;

	// 投げ縄
	Riata* riata_;


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

		bool canMove;
	};

	int moveGridMaxIndex_;
	std::vector<CanMoveGrid> canMoveGrid_;

	bool isMoveIdle_;
	bool isMove_;
	float movingTime_;


public:
	// Constructor & Destructor
	Cowherd();
	~Cowherd();

	// default method
	void Init();
	void Update();
	void Draw() override;
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
	void IsMoveIdleReSet();

	void DebugDraw();

	// 中心点のアドレスを計算
	void CenterAddUpdate();

	void RiataIsIdleTrue();
	void RiataIsStartTrue();

public: // accessor ------------------------

	// centerAdd
	Vec2 GetCenterAdd() const { return centerAdd_; }

	// isMoveIdle
	bool GetIsMoveIdle() const { return isMoveIdle_; }

	std::vector<CanMoveGrid> GetCanMoveGrid() const { return canMoveGrid_; }
	void SetCanMove(bool canMove, int index) { canMoveGrid_[index].canMove = canMove; }

	int GetCanMoveGirdMaxIndex() const { return moveGridMaxIndex_; }

	bool GetIsMove() const { return isMove_; }

	void SetWorldPos(const Vec2f& pos) { worldCenterPos_ = pos; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	void SetIsMoveIdle(bool isMoveIdle) { isMoveIdle_ = isMoveIdle; }

	bool GetCh_isMove() { return ch_isMove_; }
	void SetCh_isMove(bool ch_isMove) { ch_isMove_ = ch_isMove; }

};

