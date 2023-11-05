#include "Drawable.h"

Drawable::~Drawable() {}

bool CompareZOrder(Drawable* a, Drawable* b) {
	return a->GetZOrder() < b->GetZOrder();
}