#pragma once

#include "MyVector2.h"
#include "Quad.h"
#include <Novice.h>

class Draw {
public:


	/// <summary>
	/// Novice::DrawQuadを使いやすくしたもの
	/// </summary>
	/// <param name="center"> : 描画の中心点</param>
	/// <param name="size"> : 描画する矩形の横幅、縦幅</param>
	/// <param name="scale"> : 描画する矩形の拡縮率</param>
	/// <param name="leftTop"> : 描画する画像の左上座標</param>
	/// <param name="graphSize"> : 描画する画像の大きさ</param>
	/// <param name="gh"> : 描画する画像が読み込まれた変数</param>
	/// <param name="color"> : 色</param>
	static void Quad(
		const Vec2f& center,
		const Vec2f& size,
		const Vec2f& scale,
		const Vec2f& leftTop,
		const Vec2f& graphSize,
		int gh,
		unsigned int color
	);


	/// <summary>
	/// Novice::DrawQuadを使いやすくしたもの
	/// </summary>
	/// <param name="vertex"> : 各頂点の位置</param>
	/// <param name="leftTop"> : 描画したい画像の左上頂点</param>
	/// <param name="graphSize"> : 描画したい画像の大きさ</param>
	/// <param name="gh"> : 描画したい画像が読み込まれた変数</param>
	/// <param name="color"> : 色</param>
	static void Quad(
		const QuadVerf& vertex,
		const Vec2f& leftTop,
		const Vec2f& graphSize,
		int gh,
		unsigned int color
	);



};

