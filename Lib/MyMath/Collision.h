#pragma once

#include "MyVector2.h"
#include "Quad.h"
#include "Matrix3x3.h"

#include "MyMath/MyMath.h"


namespace Collision {


	namespace Rect {
		bool Rect(const QuadVerf& ver1, const QuadVerf& ver2);
		bool Circle(const QuadVerf& ver, const Vec2f center, float distance);
		bool Line(const QuadVerf& ver, const Vec2f& start, const Vec2f& end);
		bool Point(const QuadVerf& ver, const Vec2f& point);
	}

	namespace Circle {
		bool Rect(const Vec2f center, const QuadVerf& ver, float distance);
		bool Circle(const Vec2f& center1, const Vec2f& center2, float distance);
		bool Line(const Vec2f& center, const Vec2f& start, const Vec2f& end, float distance);
		bool Point(const Vec2f& center, const Vec2f& point, float distance);
	}

	namespace Line {
		bool Rect(const Vec2f& start, const Vec2f& end, const QuadVerf& ver);
		bool Circle(const Vec2f& start, const Vec2f& end, const Vec2f& center, float distance);
		bool Line(const Vec2f& start1, const Vec2f& end1, const Vec2f& start2, const Vec2f& end2);
		bool Point(const Vec2f& start, const Vec2f& end, const Vec2f& point);
	}

	/// <summary>
	/// pos1がpos2から見て左右どちらにいるか計算
	/// </summary>
	/// <returns> 0 : 左; 1 : 右</returns>
	bool CheckLR(const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos2Dir);

};