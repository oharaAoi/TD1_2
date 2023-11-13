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


class BullFighting:
	public BaseCow,
	public BaseMap,
	public Turn {

private:

	// 進行方向にブロックがあるかないか(4/方向,3/見る範囲)
	bool isMoveDireBlock_[4][3];

	bool moveDireOnPreson_[4][3];

	// 5つの方向に人がいるか
	std::vector<bool>allDireOnPreson_[5];

	// すべての人が斜めにいたら
	bool allPersonOnSlant_;

	// 同値があるか
	bool isSameDireValue_;

	// csvを読み込む物
	std::vector<std::vector<int>>moveGrid_;

	std::vector<std::vector<int>>direGrid_;

	// csv上の牛のアドレス
	Vec2 localCenterAdd_;

	// 全方向の牛のアドレス
	Vec2 allLocalCenterAdd_;

	// 4方向の評価する方向のアドレスの数
	int direAddressNum_[4];

	// 5方向を評価するアドレスの数
	int allDireAddressNum_[5];

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

	Address cannotMove_[4];

	MoveArea moveAreas_[4];

	Address onPresonDire_[5];

	//==========================================================
	bool isStan_;

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

	void MoveDireInit();
	void FourDireGridUpdate();

	void OnPresonInit();
	void AllDireGridUpdate();

	void MoveTurn();

	void CheckMoveDire();

	void Move();

	bool CheckIsStan();

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

	// スタン
	void SetIsStan(bool isStan) { isStan_ = isStan; }
	bool GetIsStan() { return isStan_; }

	void SetFenceAttack(bool isAttack) { isFenceAttack_ = isAttack; }
	bool GetIsFenceAttack() { return isFenceAttack_; }

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

	// 牛の4方向と斜めを調べるために使う
	Vec2 GetAllDireGridAddress(int dire, int index) { return onPresonDire_[dire].worldAdd[index]; }
	int GetAllDireGridAddressNum(int index) { return allDireAddressNum_[index]; }

	// 周りにブロックがあったら
	void SetIsMoveDireBlock(int dire, int index, bool isBlock) { isMoveDireBlock_[dire][index] = isBlock; }
	bool GetIsMoveDireBlock(int dire, int index) { return isMoveDireBlock_[dire][index]; }

	void SetIsMoveDirePreson(int dire, int index, bool isBlock) { moveDireOnPreson_[dire][index] = isBlock; }
	bool GetIsMoveDirePreson(int dire, int index) { return moveDireOnPreson_[dire][index]; }

	// 人がいたら
	void SetIsAllDireOnPerson(int dire, int index, bool isOnPreson) { allDireOnPreson_[dire][index] = isOnPreson; }
	bool GetIsOnPreson(int dire, int index) { return allDireOnPreson_[dire][index]; }

	// 人が全員斜めにいたら
	void SetAllPersonOnSlant(int isSlant) { allPersonOnSlant_ = isSlant; }
	bool GetAllPersonOnSlant() { return allPersonOnSlant_; }

	// 評価値で同値があったら
	void SetIsSameDireValue(bool isSameValue) { isSameDireValue_ = isSameValue; }
	bool GetIsSameDireValue() { return isSameDireValue_; }

};

