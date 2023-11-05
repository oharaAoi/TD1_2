#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::AddDrawable(Drawable* drawable) {
	drawables.push_back(drawable);
}

void Renderer::Draw() {
	std::sort(drawables.begin(), drawables.end(), CompareZOrder);
	for (Drawable* drawable : drawables) {
		drawable->Draw();
	}
}
