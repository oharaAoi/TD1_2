﻿#pragma once
#include "ImGuiManager.h"

// ----- object ----- //
#include "BaseCow.h"
#include "BaseMap.h"
#include "MapChip.h"

// ----- Maneger ----- //
#include "Turn.h"

#include "MyMath.h"


class BullFighting:
	public BaseCow,
	public BaseMap,
	public Turn {

private:

	// 進行方向にブロックがあるかないか(4/方向,3/見る範囲)
	bool isMoveDireBlock_[4][3];

	bool moveDireOnPreson_[4][3];

	// csvを読み込む物
	std::vector<std::vector<int>>moveGrid_;

	// csv上の牛のアドレス
	Vec2 localCenterAdd_;

	// 8方向の評価する方向のアドレスの数
	int direAddressNum_[4];

	struct Address {
		std::vector<Vec2> localAdd;
		std::vector<Vec2> worldAdd;
	};

	Address cannotMove_[4];

public:

	BullFighting();
	~BullFighting();

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
	acsseser
================================================================*/
	// pos
	Vec2f GetWorldPos() { return worldPos_; }
	void SetWorldPos(Vec2f pos) { worldPos_ = pos; }

	// アドレス
	Vec2 GetWorldAdd() { return worldAdd_; }

	// 移動した方向を取得
	kCanMoveDirection GetMovedDire() { return movedDire_; }

	// 移動する
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	bool GetIsMove() { return isMove_; }

	// 評価値を入れる配列
	void SetMoveDireValue(int value, int num) { canMoveDireValue_[num] = value; }
	int GetMoveDireValue(int num)const { return canMoveDireValue_[num]; }

	// 評価値
	int GetAllDireValue() { return value_.allDire; }
	int GetFourAreaValue() { return value_.fourArea; }
	int GetClampValue() { return value_.clamp; }
	int GetDogValue() { return value_.dog; }
	int GetFenceValue() { return value_.fence; }
	int GetSlantFenceValue() { return value_.slantFence; }
	int GetAdjoinValue() { return value_.adjoin; }
	int GetAdjoinAddValue() { return value_.adjoinAdd; }
	int GetRockValue() { return value_.rock; }

	// 周りを評価するのに使う
	Vec2 GetCantMoveAdd(int dire, int index) { return cannotMove_[dire].worldAdd[index]; }
	int GetDireAddressNum(int index) { return direAddressNum_[index]; }

	// 周りにブロックがあったら
	void SetIsMoveDireBlock(int dire, int index, bool isBlock) { isMoveDireBlock_[dire][index] = isBlock; }
	bool GetIsMoveDireBlock(int dire, int index) { return isMoveDireBlock_[dire][index]; }

	void SetIsMoveDirePreson(int dire, int index, bool isBlock) { moveDireOnPreson_[dire][index] = isBlock; }
	bool GetIsMoveDirePreson(int dire, int index) { return moveDireOnPreson_[dire][index]; }
};

