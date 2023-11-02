#pragma once
#include <Novice.h>
#include <vector>
#include <string>

#include "MyVector2.h"
#include "Quad.h"
#include "Matrix3x3.h"
#include "Lib/LoadFile/LoadFile.h"

enum ChipType {
	NONE,
	STAGEOUT,
	FENCE,
	COWHERD,
	COW
};

class MapChip
{

private:

	//==================================================
	//マップのアドレス
	std::vector<std::vector<int>>mapAdd_;

	//マップのファイルパス
	std::string csvFilePath_[10];

	//行と列
	int row_;
	int col_;

	//マップの画像
	int GH_;

	//マップの画像の番号の振り分け
	int mapTileWidth_[3];
	int mapTileHeight_[3];

	//1タイル当たりの大きさ
	Vec2f size_;
	//==================================================

	struct Base {
		Vec2f pos;
		Vec2f scall;

		ChipType type;

		//==================================================
		//矩形
		QuadVerf worldVertex;

		QuadVerf screenVertex;

		//==================================================

	};

	Base** mapChip_;

	//原点中心vertex
	QuadVerf localVertex_;

	//==================================================

	Matrix3x3 worldMatrix_;

	Matrix3x3 screenMatrix_;


public:

	MapChip();
	~MapChip();

	//===========================================
	void Init();
	void Update();
	void Draw();

	//===========================================

	void CalcWorldVertex();
	void CalcScreenVertex();

	//===========================================

	void CoordinateChange(const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport);

	/*アクセッサ*/
	Vec2f GetPos(int row, int col) { return mapChip_[row][col].pos; }

};

