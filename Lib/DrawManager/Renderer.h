#pragma once
#include "Drawable.h"
#include <vector>
#include <algorithm>

class Renderer {
private:

	std::vector<Drawable*> drawables;

public:
	Renderer();
	~Renderer();

	void ResetDrawable();
	void AddDrawable(Drawable* drawable);

	void Draw();

};

