#pragma once
#include <Novice.h>
#include <vector>
#include <string>

/* Lib */
#include "MyVector2.h"
#include "Quad.h"
#include "Matrix3x3.h"
#include "Lib/LoadFile/LoadFile.h"
#include "Draw.h"

/* object */
#include "BaseMap.h"

enum ChipType {
	NONE,		// 何もない
	STAGEOUT,	// フェンスの外
	FENCE,		// フェンス
	COWHERD,	// 牛飼いの初期位置
	YANGMAN,	// 若人の初期位置
	COW,		// 牛の初期位置
	ROCK,		// 岩
};

class MapChip final
	: public BaseMap {
private:

	//==================================================
	//マップのアドレス
	/*std::vector<std::vector<int>>mapAdd_;*/

	//マップのファイルパス
	static const int kMaxStageNo_ = 10;
	std::string csvFilePath_[kMaxStageNo_];

	//マップの画像の番号の振り分け
	Vec2f mapTile_[3][3];

	//1タイル当たりの大きさ
	Vec2f size_;
	//==================================================
	// マップチップ
	struct Base final {
		Vec2f pos;
		Vec2f scale;

		unsigned int color;

		ChipType type;

		// 各頂点
		QuadVerf worldVertex;

		QuadVerf screenVertex;

	};

	Base** mapChip_;

	//原点中心vertex
	QuadVerf localVertex_;

	// Cowheadのpos
	Vec2f cowheadPos_;

	// Cowのpos
	Vec2f cowPos_;

	//==================================================
	// 行列
	Matrix3x3 worldMatrix_;

	Matrix3x3 screenMatrix_;

	//==================================================
	//デバック用
	// line
	struct Line {
		Vec2f st;
		Vec2f end;
	};

	Line xAxis_[10];
	Line yAxis_[10];

public:

	MapChip();
	~MapChip();

	//===========================================
	void Init();
	void Update();
	void Draw();

	//===========================================

	void DebugInit();
	void DebugDraw();

	//===========================================

	void CalcWorldVertex();
	void CalcScreenVertex();

	//===========================================

	void MatrixChange(const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport);
	void MakeWorldMatrix();

	/* アクセッサ */
	Vec2f GetPos(int row, int col) { return mapChip_[row][col].pos; }

	Vec2f GetCowheadPos() { return cowheadPos_; }
	Vec2f GetCowPos() { return cowPos_; }

	//マップのアドレスを返す(playerやcowの初期化で使う)
	std::vector<std::vector<int>>GetMapChipAdd() { return mapAdd_; }

	int GetMapChipRow() { return row_; }
	int GetMapChipCol() { return col_; }

	Vec2f GetTileSize() { return size_; }

};

