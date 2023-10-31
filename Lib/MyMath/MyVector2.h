#pragma once


template <typename T> 

/// <summary>
/// ベクトル
/// </summary>
class Vector2 final {
public:

	T x, y;

	// 四則演算のオペレーター;
	Vector2 operator+(const Vector2& other) const {
		return Vector2{ this->x + other.x , this->y + other.y };
	}
	Vector2 operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	Vector2 operator-(const Vector2& other) const {
		return Vector2{ this->x - other.x , this->y - other.y };
	}
	Vector2 operator-=(const Vector2& other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	Vector2 operator*(const Vector2& other) const {
		return Vector2{ this->x * other.x , this->y * other.y };
	}
	Vector2 operator*=(const Vector2& other) {
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}
	Vector2 operator/(const Vector2& other) const {
		return Vector2{ this->x / other.x , this->y / other.y };
	}
	Vector2 operator/=(const Vector2& other) {
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}
};


using Vec2 = Vector2<int>;
using Vec2f = Vector2<float>;
using Vec2lf = Vector2<double>;