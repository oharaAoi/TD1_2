#include "Draw.h"

void Draw::Quad(
	const Vec2f& center,
	const Vec2f& size,
	const Vec2f& scale,
	const Vec2f& leftTop,
	const Vec2f& graphSize,
	int gh,
	unsigned int color) {

	QuadVerf ver{
		{center.x - (size.x * 0.5f * scale.x), center.y + (size.y * 0.5f * scale.y)},
		{center.x + (size.x * 0.5f * scale.x), center.y + (size.y * 0.5f * scale.y)},
		{center.x - (size.x * 0.5f * scale.x), center.y - (size.y * 0.5f * scale.y)},
		{center.x + (size.x * 0.5f * scale.x), center.y - (size.y * 0.5f * scale.y)},
	};

	Novice::DrawQuad(
		static_cast<int>(ver.lt.x),
		static_cast<int>(ver.lt.y),
		static_cast<int>(ver.rt.x),
		static_cast<int>(ver.rt.y),
		static_cast<int>(ver.lb.x),
		static_cast<int>(ver.lb.y),
		static_cast<int>(ver.rb.x),
		static_cast<int>(ver.rb.y),
		static_cast<int>(leftTop.x),
		static_cast<int>(leftTop.y),
		static_cast<int>(graphSize.x),
		static_cast<int>(graphSize.y),
		gh,
		color
	);

}

void Draw::Quad(const QuadVerf& vertex, const Vec2f& leftTop, const Vec2f& graphSize, int gh, unsigned int color) {

	Novice::DrawQuad(
		static_cast<int>(vertex.lt.x),
		static_cast<int>(vertex.lt.y),
		static_cast<int>(vertex.rt.x),
		static_cast<int>(vertex.rt.y),
		static_cast<int>(vertex.lb.x),
		static_cast<int>(vertex.lb.y),
		static_cast<int>(vertex.rb.x),
		static_cast<int>(vertex.rb.y),
		static_cast<int>(leftTop.x),
		static_cast<int>(leftTop.y),
		static_cast<int>(graphSize.x),
		static_cast<int>(graphSize.y),
		gh,
		color
	);

}
