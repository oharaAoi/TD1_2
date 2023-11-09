#pragma once
#include <vector>
#include <string>
#include <Novice.h>

/* lib */
#include "MyVector2.h"
#include "Lib/LoadFile/LoadFile.h"

class BaseMap
{
protected:
	//==================================================
	// マップのアドレス
	static std::vector<std::vector<int>> mapAdd_;
	static std::vector<std::vector<int>> nowMapAdd_;

	// マップのファイルパス
	static std::string csvFilePath_;

	// 行と列
	static int row_;
	static int col_;

	// タイルのサイズ
	static Vec2f tileSize_;

	// リソース
	static int GH_;

	//==================================================

public:
	// コンストラクタ
	BaseMap();

	static void AddressRevers();
	static void Init();

	//*ここから先は継承先でも使う*/
	// デストラクタ
	virtual ~BaseMap();

	static void Swap(const Vec2& a, const Vec2& b);

};

