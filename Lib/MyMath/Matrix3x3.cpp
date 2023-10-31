#include "Matrix3x3.h"


#include <Novice.h>
#include <math.h>
#include <assert.h>



/// <summary>
/// 3x3の行列のデバッグ表示
/// </summary>
void MatrixScreenPrintf(int x, int y, const Matrix3x3& matrix) {
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {

			Novice::ScreenPrintf(
				x + col * kColWidth,
				y + row * kRowHeight,
				"%0.2f", matrix.m[row][col]
			);
		}
	}
}


/// <summary>
/// 3x3の行列の積
/// </summary>
Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2) {
	Matrix3x3 result;

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {

			//積の結果を代入
			result.m[row][col] =
				matrix1.m[row][0] * matrix2.m[0][col]
				+ matrix1.m[row][1] * matrix2.m[1][col]
				+ matrix1.m[row][2] * matrix2.m[2][col];
		}
	}

	return result;
}


/// <summary>
/// 3x3の行列の回転
/// </summary>
Matrix3x3 MakeRotateMatrix(float theta) {
	Matrix3x3 result;

	result.m[0][0] = cosf(theta);
	result.m[0][1] = -sinf(theta);
	result.m[0][2] = 0;

	result.m[1][0] = sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[1][2] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;

	return result;
}


/// <summary>
/// 3x3の行列の回転
/// </summary>
Matrix3x3 MakeRotateMatrix(const Vec2f& dir) {
	Matrix3x3 result;

	result.m[0][0] = dir.x;
	result.m[0][1] = -dir.y;
	result.m[0][2] = 0;

	result.m[1][0] = dir.y;
	result.m[1][1] = dir.x;
	result.m[1][2] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;

	return result;
}


/// <summary>
/// 3x3の行列の拡縮
/// </summary>
Matrix3x3 MakeScaleMatrix(const Vec2f& scale) {
	Matrix3x3 result;

	result.m[0][0] = scale.x;
	result.m[0][1] = 0;
	result.m[0][2] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;

	return result;
}


/// <summary>
/// 3x3行列の平行移動
/// </summary>
Matrix3x3 MakeTranslateMatrix(const Vec2f& translate) {
	Matrix3x3 result;

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;

	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;

	return result;
}


/// <summary>
/// 2次元ベクトルの移動
/// </summary>
Vec2f Transform(const Vec2f& vector, const Matrix3x3& matrix) {
	//w=1がデカルト座標系であるので(x,y,1)のベクトルとしてmatrixとの積をとる
	Vec2f result{ 0.0f,0.0f };

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];

	//ベクトルに対して基本的な操作を行う行列でwが0になることはありえない
	//wが0.0fになった場合プログラムを停止する
	assert(w != 0.0f);

	//w=1がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	result.x /= w;
	result.y /= w;

	return result;
}

Matrix3x3 MakeAffineMatrix(Vec2f scale, float theta, Vec2f translate) {
	Matrix3x3 scaleMatrix = MakeScaleMatrix(scale);
	Matrix3x3 rotateMatrix = MakeRotateMatrix(theta);
	Matrix3x3 translateMatrix = MakeTranslateMatrix(translate);

	Matrix3x3 result = Multiply(scaleMatrix, rotateMatrix);
	result = Multiply(result, translateMatrix);

	return result;
}

Matrix3x3 MakeOrthoMatrix(float top, float bottom, float left, float right) {
	Matrix3x3 result;

	result.m[0][0] = 2.0f / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[1][2] = 0;

	result.m[2][0] = (left + right) / (left - right);
	result.m[2][1] = (top + bottom) / (bottom - top);
	result.m[2][2] = 1.0f;

	return result;
}

Matrix3x3 MakeViewportMatrix(float top, float left, float width, float height) {
	Matrix3x3 result;

	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -(height / 2.0f);
	result.m[1][2] = 0.0f;

	result.m[2][0] = left + (width / 2.0f);
	result.m[2][1] = top + (height / 2.0f);
	result.m[2][2] = 1.0f;

	return result;
}

Matrix3x3 MakeInverseMatrix(const Matrix3x3& matrix) {
	Matrix3x3 result{ 0.0f };
	float divide =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2]
		+ matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0]
		+ matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1]
		- matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]
		- matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2]
		- matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1];

	assert(divide != 0.0f);


	result.m[0][0] = 1.0f / divide * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
	result.m[0][1] = 1.0f / divide * -(matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]);
	result.m[0][2] = 1.0f / divide * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);

	result.m[1][0] = 1.0f / divide * -(matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]);
	result.m[1][1] = 1.0f / divide * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
	result.m[1][2] = 1.0f / divide * -(matrix.m[0][0] * matrix.m[0][2] - matrix.m[0][2] * matrix.m[1][0]);

	result.m[2][0] = 1.0f / divide * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
	result.m[2][1] = 1.0f / divide * -(matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]);
	result.m[2][2] = 1.0f / divide * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);


	return result;
}

Matrix3x3 MakeWvpVpMatrix(const Matrix3x3& world, const Matrix3x3& view, const Matrix3x3& ortho, const Matrix3x3& viewport) {
	Matrix3x3 result = Multiply(world, view);
	result = Multiply(result, ortho);
	result = Multiply(result, viewport);

	return result;
}

