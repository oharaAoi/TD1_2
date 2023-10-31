#pragma once
#include "MyVector2.h"

template <typename T>

/// <summary>
/// 各頂点
/// </summary>
class QuadVertex final {
public:
	
	// 各頂点の変数
	T lt;
	T rt;
	T lb;
	T rb;

	// 四則演算のオペレーター
	QuadVertex operator+(const QuadVertex& other)const {
		QuadVertex result;
		result.lt = this->lt + other.lt;
		result.rt = this->rt + other.rt;
		result.lb = this->lb + other.lb;
		result.rb = this->rb + other.rb;
		return result;
	}
	void operator+=(const QuadVertex& other) {
		this->lt += other.lt;
		this->rt += other.rt;
		this->lb += other.lb;
		this->rb += other.rb;
	}
	QuadVertex operator-(const QuadVertex& other) const {
		QuadVertex result;
		result.lt = this->lt - other.lt;
		result.rt = this->rt - other.rt;
		result.lb = this->lb - other.lb;
		result.rb = this->rb - other.rb;
		return result;
	}
	void operator-=(const QuadVertex& other) {
		this->lt -= other.lt;
		this->rt -= other.rt;
		this->lb -= other.lb;
		this->rb -= other.rb;
	}
	QuadVertex operator/(const QuadVertex& other) const {
		QuadVertex result;
		result.lt = this->lt / other.lt;
		result.rt = this->rt / other.rt;
		result.lb = this->lb / other.lb;
		result.rb = this->rb / other.rb;
		return result;
	}
	void operator/=(const QuadVertex& other) {
		this->lt /= other.lt;
		this->rt /= other.rt;
		this->lb /= other.lb;
		this->rb /= other.rb;
	}
	QuadVertex operator*(const QuadVertex& other) const {
		QuadVertex result;
		result.lt = this->lt / other.lt;
		result.rt = this->rt / other.rt;
		result.lb = this->lb / other.lb;
		result.rb = this->rb / other.rb;
		return result;
	}
	void operator*=(const QuadVertex& other) {
		this->lt *= other.lt;
		this->rt *= other.rt;
		this->lb *= other.lb;
		this->rb *= other.rb;
	}
	
};


// 
using QuadVer = QuadVertex<Vec2>;
using QuadVerf = QuadVertex<Vec2f>;
using QuadVerlf = QuadVertex<Vec2lf>;