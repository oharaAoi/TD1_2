#include "Renderer.h"

Renderer::Renderer() { ResetDrawable(); }

Renderer::~Renderer() {}

void Renderer::ResetDrawable() {
	drawables.resize(0);
}

void Renderer::AddDrawable(Drawable* drawable) {
	drawables.push_back(drawable);
}

void Renderer::Draw() {
	std::sort(drawables.begin(), drawables.end(), CompareZOrder);
	for (Drawable* drawable : drawables) {
		drawable->Draw();
	}
}
