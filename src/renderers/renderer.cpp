#include "renderers/renderer.h"

Scene* Renderer::GetScene() {
	return this->scene;
}

void Renderer::SetScene(Scene* scene) {
	this->scene = scene;
}
