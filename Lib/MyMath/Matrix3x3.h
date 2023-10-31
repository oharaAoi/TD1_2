#pragma once
#include "MyVector2.h"

/// <summary>
/// 3x3の行列
/// </summary>
class Matrix3x3 final {
public:

	// 行列
	float m[3][3];

	// 四則演算のオペレーター; 除算を除く
	Matrix3x3 operator+ (const Matrix3x3& other) const {
		Matrix3x3 result;
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				result.m[row][col] = this->m[row][col] + other.m[row][col];
			}
		}
		return result;
	}
	Matrix3x3 operator+= (const Matrix3x3& other) {
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				this->m[row][col] += other.m[row][col];
			}
		}
		return *this;
	}
	Matrix3x3 operator- (const Matrix3x3& other) const {
		Matrix3x3 result;
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				result.m[row][col] = this->m[row][col] - other.m[row][col];
			}
		}
		return result;
	}
	Matrix3x3 operator-= (const Matrix3x3& other) {
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				this->m[row][col] -= other.m[row][col];
			}
		}
		return *this;
	}
	Matrix3x3 operator* (const Matrix3x3& other) const {
		Matrix3x3 result;
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				result.m[row][col] =
					this->m[row][0] * other.m[0][col]
					+ this->m[row][1] * other.m[1][col]
					+ this->m[row][2] * other.m[2][col];
			}
		}
		return result;
	}
	Matrix3x3 operator*= (const Matrix3x3& other) {
		Matrix3x3 result;
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				result.m[row][col] =
					this->m[row][0] * other.m[0][col]
					+ this->m[row][1] * other.m[1][col]
					+ this->m[row][2] * other.m[2][col];
			}
		}
		*this = result;
		return *this;
	}
};


static const int kRowHeight = 20;
static const int kColWidth = 60;

/// <summary>
/// 3x3の行列のデバッグ表示
/// </summary>
void MatrixScreenPrintf(int x, int y, const Matrix3x3& matrix);


/// <summary>
/// 3x3の行列の積
/// </summary>
Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2);


/// <summary>
/// 3x3の行列の回転
/// </summary>
Matrix3x3 MakeRotateMatrix(float theta);


/// <summary>
/// 3x3の行列の回転
/// </summary>
Matrix3x3 MakeRotateMatrix(const Vec2f& dir);


/// <summary>
/// 3x3の行列の拡縮
/// </summary>
Matrix3x3 MakeScaleMatrix(const Vec2f& scale);


/// <summary>
/// 3x3行列の平行移動
/// </summary>
Matrix3x3 MakeTranslateMatrix(const Vec2f& translate);


/// <summary>
/// 2次元ベクトルの移動
/// </summary>
Vec2f Transform(const Vec2f& vector, const Matrix3x3& matrix);


/// <summary>
/// WorldMatrixの生成; SRTを同時
/// </summary>
/// <param name="scale">拡縮率</param>
/// <param name="theta">回転角</param>
/// <param name="translate">平行移動</param>
/// <returns></returns>
Matrix3x3 MakeAffineMatrix(Vec2f scale, float theta, Vec2f translate);


/// <summary>
/// 正射影行列の生成
/// </summary>
/// <param name="top">切り取る範囲の上の値</param>
/// <param name="bottom">切り取る範囲の下の値</param>
/// <param name="left">切り取る範囲の左の値</param>
/// <param name="right">切り取る範囲の右の値</param>
/// <returns></returns>
Matrix3x3 MakeOrthoMatrix(float top, float bottom, float left, float right);


/// <summary>
/// NDCの生成; ビューポート行列の生成
/// </summary>
/// <param name="top"></param>
/// <param name="left"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Matrix3x3 MakeViewportMatrix(float top, float left, float width, float height);


/// <summary>
/// 逆行列の生成
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix3x3 MakeInverseMatrix(const Matrix3x3& matrix);


/// <summary>
/// wvpVp行列の生成; worldMatrixをスクリーン空間の座標に変換
/// </summary>
/// <param name="world"></param>
/// <param name="view"></param>
/// <param name="ortho"></param>
/// <param name="viewport"></param>
/// <returns></returns>
Matrix3x3 MakeWvpVpMatrix(const Matrix3x3& world, const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport);



