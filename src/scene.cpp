#include "scene.h"

Scene::Scene() {}

Scene::Scene(Mesh* mesh) {
	this->mesh = mesh;
	this->Init();
}

Scene::~Scene() {
	this->Clean();
}

bool Scene::Render(Shader* shader, ImVec2 size) {
	if (this->mesh == nullptr)
		return false;
	if (this->camera == nullptr)
		return false;

	this->camera->SetScreenViewport(Eigen::AlignedBox2f(
			Eigen::Vector2f(0., 0.),
			Eigen::Vector2f(size.x, size.y)));

	glBindVertexArray(this->vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID[1]);

	int vertexLocation = shader->getAttribLocation("vtx_position");
	if (vertexLocation >= 0) {
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) 0));
		glEnableVertexAttribArray(vertexLocation);
	}

	int colorLocation = shader->getAttribLocation("vtx_color");
	if (colorLocation >= 0) {
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) sizeof(Eigen::Vector3f)));
		glEnableVertexAttribArray(colorLocation);
	}

	int normalLocation = shader->getAttribLocation("vtx_normal");
	if (normalLocation >= 0) {
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) (2 * sizeof(Eigen::Vector3f))));
		glEnableVertexAttribArray(normalLocation);
	}

	glDrawElements(GL_TRIANGLES, (3 * this->mesh->nbFaces), GL_UNSIGNED_INT, 0);

	if (vertexLocation >= 0)
		glDisableVertexAttribArray(vertexLocation);
	if (colorLocation >= 0)
		glDisableVertexAttribArray(colorLocation);
	if (normalLocation >= 0)
		glDisableVertexAttribArray(normalLocation);

	glBindVertexArray(0);

	return true;
}

void Scene::MoveCameraPosition(Eigen::Vector3f direction) {
	this->cameraPosition += direction;
	this->UpdateViewMatrix();
}

Camera* Scene::GetCamera() {
	return this->camera;
}

Mesh* Scene::GetMesh() {
	return this->mesh;
}

Eigen::Matrix4f Scene::GetViewMatrix() {
	return this->viewMatrix;
}

void Scene::SetCamera(Camera* camera) {
	if (this->camera != nullptr)
		delete this->camera;
	this->camera = camera;
}

void Scene::SetMesh(Mesh* mesh) {
	if (this->mesh != nullptr)
		this->Clean();
	this->mesh = mesh;
	this->Init();
}

void Scene::Init() {
	glGenVertexArrays(1, &this->vaoID);
	glGenBuffers(2, this->vboID);

	glBindVertexArray(this->vaoID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboID[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(int) * 3 * this->mesh->nbFaces),
			this->mesh->facesVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->vboID[1]);
	glBufferData(GL_ARRAY_BUFFER,
			(sizeof(struct Vertex) * this->mesh->nbVertices),
			this->mesh->verticesData[0].position.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	this->camera = new Camera();
	this->camera->SetSceneRadius(this->mesh->GetBoundingBox()
			.sizes().maxCoeff());
	this->camera->SetSceneDistance(this->camera->GetSceneRadius() * 3.f);
	this->camera->SetMinNear(0.1f);
	this->camera->SetNearFarOffsets(-this->camera->GetSceneRadius() * 100.f,
			this->camera->GetSceneRadius() * 100.f);


	this->cameraPosition = Eigen::Vector3f(
		-2 * this->mesh->GetBoundingBox().sizes().x(),
		-2 * this->mesh->GetBoundingBox().sizes().y(),
		2 * this->mesh->GetBoundingBox().sizes().z());
	this->sceneCenter = this->mesh->GetBoundingBox().center();

	this->UpdateViewMatrix();
}

void Scene::Clean() {
	glDeleteBuffers(2, this->vboID);
	glDeleteVertexArrays(1, &this->vaoID);

	if (this->camera != nullptr) {
		delete this->camera;
		this->camera = nullptr;
	}
}

void Scene::UpdateViewMatrix() {
	this->viewMatrix = this->camera->LookAt(
			this->cameraPosition,
			this->sceneCenter,
			Eigen::Vector3f(0., 1., 0.));
}
