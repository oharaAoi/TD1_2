#pragma once
#include "MyVector2.h"
#include "Quad.h"

namespace MyMath {

	/*=============================================================
		ベクトル
	=============================================================*/

	/// <summary>
	/// Normalizeをするか
	/// </summary>
	enum NormalizationFlag {
		Normalized,
		NotNormalized
	};

	/// <summary>
	/// pos1からpos2への方向ベクトルを計算
	/// </summary>
	Vec2f Direction(const Vec2f& pos1, const Vec2f& pos2, NormalizationFlag normalizationFlag);


	/// <summary>
	/// vectorを正規化する
	/// </summary>
	Vec2f Normalize(const Vec2f& vector);


	/// <summary>
	/// pos1からpos2の長さを計算する
	/// </summary>
	float Length(const Vec2f& pos1, const Vec2f& pos2);


	/// <summary>
	/// pos1->pos2への距離を計算する
	/// </summary>
	Vec2f Distance(const Vec2f& pos1, const Vec2f& pos2);


	/// <summary>
	/// 内積の計算
	/// </summary>
	float DotProduct(const Vec2f& dir1, const Vec2f& dir2);


	/// <summary>
	/// 外積の計算
	/// </summary>
	float CrossProduct(const Vec2f& dir1, const  Vec2f& dir2);


	/*=============================================================

	=============================================================*/

	/// <summary>
	/// 線形補完
	/// </summary>
	/// <param name="t">媒介変数 : 1 ~ 0</param>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <returns>始点から終点のt分の値</returns>
	float Lerp(float t, float start, float end);


}