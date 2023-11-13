#pragma once
#include "ImGuiManager.h"

// ----- object ----- //
#include "BaseCow.h"
#include "BaseMap.h"
#include "MapChip.h"

// ----- Maneger ----- //
#include "Turn.h"

// ----- lib ----- //
#include "MyMath.h"

class BullCow : 
	public BaseCow,
	public BaseMap,
	public Turn {

private:

	bool isDisplay_;

	// csvを読み込む物
	std::vector<std::vector<int>>cannotMoveGrid_;

	Vec2 localCenterAdd_;

	// 
	int direAddressNum_[8];

	struct Address {
		std::vector<Vec2> localAdd;
		std::vector<Vec2> worldAdd;
	};

	struct MoveArea {
		std::vector<Vec2f>worldPos;

		std::vector<QuadVerf> screenVertex;

		std::vector<Matrix3x3> screenMatrix;
		std::vector<Matrix3x3> worldMatrix;
	};

	Address cannotMove_[8];

	MoveArea moveAreas_[8];


public:

	BullCow();
	~BullCow();


/*================================================================
	 default method
================================================================*/
	void Init();
	void Update()override;
	void Draw()override;
	void Finalize()override;

/*================================================================
	 user method
================================================================*/
	void CenterAddUpdate();

	void DireInit();

	void MoveTurn();

	void CheckMoveDire();

	void Move();

	// 動ける範囲の座標を求める
	void MoveAreasUpdate();

	// スクリーン行列と各頂点の計算
	void MatrixChange(
		const Matrix3x3& viewMatrix,
		const Matrix3x3& orthoMatrix,
		const Matrix3x3& viewportMatrix
	);

	// ワールド行列と各頂点の計算
	void MakeWorldMatrix();

	// imguiの表示
	void ImguiDraw();

/*================================================================
	 user method
================================================================*/
	void SetWorldPos(Vec2f pos) { worldPos_ = pos; }
	Vec2f GetWorldPos() { return worldPos_; }

	Vec2 GetCenterAdd() { return worldAdd_; }

	void SetIsMove(bool isMove) { isMove_ = isMove; }
	bool GetIsMove() { return isMove_; }

	// 移動した方向を取得
	kCanMoveDirection GetMovedDire() { return movedDire_; }

	// 方向を決めるための量
	void SetMoveDireValue(int value, int num) { canMoveDireValue_[num] = value; }
	int GetMoveDireValue(int num)const { return canMoveDireValue_[num]; }

	// 牛から見たプレイヤー達の位置を計算するために使う
	void SetGridDistanceValue(int value, int dire) { gridDistanceValue_[dire] = value; }
	int GetGridDistanceValue(int dire) { return gridDistanceValue_[dire]; }

	// フェンスを攻撃するかどうかで使う
	void SetIsFenceAttack(bool isAttack) { isFenceAttack_ = isAttack; }
	bool GetIsFenseAttack() { return isFenceAttack_; }

	// 評価値
	int GetDogValue() { return value_.dog; }
	int GetAllDireValue() { return value_.allDire; }
	int GetFourAreaValue() { return value_.fourArea; }
	int GetAdjoinValue() { return value_.adjoin; }
	int GetAdjoinAddValue() { return value_.adjoinAdd; }

	int GetClampValue() { return value_.clamp; }
	int GetSlantFenceValue() { return value_.slantFence; }

	int GetFenceValue() { return value_.fence; }
	int GetRockValue() { return value_.rock; }

	int GetWallValue() { return value_.wall; }

	void SetFenceValue(int value) { value_.fence = value; }

	// 周りを評価するのに使う
	Vec2 GetCantMoveAdd(int dire, int index) { return cannotMove_[dire].worldAdd[index]; }
	int GetDireAddressNum(int index) { return direAddressNum_[index]; }
};