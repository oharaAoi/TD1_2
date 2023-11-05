#pragma once


class Drawable {
private:

	int zOrder_;

public:
	virtual ~Drawable();

	int GetZOrder() const { return zOrder_; }
	void SetZOder(int z) { zOrder_ = z; }

	virtual void Draw() = 0;

};

bool CompareZOrder(Drawable* a, Drawable* b);