#include "MyMath.h"
#include <Novice.h>
#include <math.h>


/*=============================================================
	ベクトル
=============================================================*/


Vec2f MyMath::Direction(const Vec2f& pos1, const Vec2f& pos2, NormalizationFlag normalizationFlag) {
	if (normalizationFlag == NormalizationFlag::Normalized) { return Normalize(pos2 - pos1); }
	return pos2 - pos1;
}


Vec2f MyMath::Normalize(const Vec2f& vector) {
	//vectorの長さを計算
	Vec2f result = vector;
	float length = sqrtf(powf(vector.x, 2.0f) + powf(vector.y, 2.0f));

	//0割り禁止
	if (length != 0) {
		result.x /= length;
		result.y /= length;
	}

	return result;
}


float MyMath::Length(const Vec2f& pos1, const Vec2f& pos2) {
	return sqrtf(powf(pos2.x - pos1.x, 2.0f) + powf(pos2.y - pos1.y, 2.0f));;
}


Vec2f MyMath::Distance(const Vec2f& pos1, const Vec2f& pos2) {
	return pos2 - pos1;
}


float MyMath::DotProduct(const Vec2f& dir1, const Vec2f& dir2) {
	return ((dir1.x * dir2.x) + (dir1.y * dir2.y));
}


float MyMath::CrossProduct(const Vec2f& dir1, const  Vec2f& dir2) {
	return dir1.x * dir2.y - dir1.y * dir2.x;
}


/*=============================================================

=============================================================*/

float MyMath::Lerp(float t, float start, float end) {
	return (1.0f - t) * start + t * end;
}