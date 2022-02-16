#include "scene.h"

Scene::Scene() {}

Scene::~Scene() {}

Camera* Scene::GetCamera() {
	return this->camera;
}

Mesh* Scene::GetMesh() {
	return this->mesh;
}

void Scene::SetCamera(Camera* camera) {
	this->camera = camera;
}

void Scene::SetMesh(Mesh* mesh) {
	this->mesh = mesh;
}
