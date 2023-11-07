#pragma once


struct Color {
	unsigned int color[4];//0 RED 1 GREEN 2 BLUE 3 ALPHA
};

/// <summary>
/// 値をカラーコードに変換
/// </summary>
/// <param name="befor">返換前の構造体</param>
/// <returns></returns>
unsigned int DegreeColorTranceforme(const Color& befor);

/// <summary>
/// 色のグラデーション
/// </summary>
/// <param name="befor">返換前の色</param>
/// <param name="after">返換後の色</param>
/// <param name="raito">前後の割合</param>
/// <param name="max">割合の最大</param>
/// <returns></returns>
unsigned int ColorGradation(const Color& befor, const Color& after, float raito, float max);
