#include "scene.h"

Scene::Scene()
		: camera(new Camera()) {
	// TODO: To remove, only for test purpose
	this->lights.push_back(new DirectionalLight(
			Eigen::Vector3f(1., 1., 1.),
			Eigen::Vector3f(0., 1., 0.)));
	this->lights.push_back(new DirectionalLight(
			Eigen::Vector3f(1., 0., 0.),
			Eigen::Vector3f(0., -1., 0.)));
}

Scene::Scene(Mesh* mesh) {
	this->mesh = mesh;
	this->Init();
}

Scene::~Scene() {
	for (auto i: this->lights)
		delete i;

	this->Clean();
}

bool Scene::RenderMesh(ShaderReader* shader) {
	if (this->mesh == nullptr)
		return false;

	glBindVertexArray(this->vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID[1]);

	int vertexLocation = shader->GetAttribLocation("vtx_position");
	if (vertexLocation >= 0) {
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) 0));
		glEnableVertexAttribArray(vertexLocation);
	}

	int colorLocation = shader->GetAttribLocation("vtx_color");
	if (colorLocation >= 0) {
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) sizeof(Eigen::Vector3f)));
		glEnableVertexAttribArray(colorLocation);
	}

	int normalLocation = shader->GetAttribLocation("vtx_normal");
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

void Scene::UpdateCameraViewport(ImVec2 size) {
	if (this->camera != nullptr) {
		this->camera->SetScreenViewport(Eigen::AlignedBox2f(
				Eigen::Vector2f(0., 0.),
				Eigen::Vector2f(size.x, size.y)));
	}
}

const Eigen::Vector3f& Scene::GetAmbientColor() {
	return this->ambientColor;
}

Camera* Scene::GetCamera() {
	return this->camera;
}

Mesh* Scene::GetMesh() {
	return this->mesh;
}

const Eigen::Matrix4f& Scene::GetMeshTransformationMatrix() {
	return this->meshTransformationMatrix;
}

Eigen::Matrix3f Scene::GetNormalMatrix() {
	Eigen::Matrix4f normalMatrix = this->camera->ComputeViewMatrix()
			* this->meshTransformationMatrix;
	return normalMatrix.block<3, 3>(0, 0).inverse().transpose();
}

std::vector<DirectionalLight*>* Scene::GetLights() {
	return &this->lights;
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
	this->camera->SetSceneCenter(this->mesh->GetBoundingBox().center());
	this->camera->SetSceneRadius(this->mesh->GetBoundingBox()
			.sizes().maxCoeff());
	this->camera->SetSceneDistance(this->camera->GetSceneRadius() * 3.f);
	this->camera->SetMinNear(0.1f);
	this->camera->SetNearFarOffsets(-this->camera->GetSceneRadius() * 100.f,
			this->camera->GetSceneRadius() * 100.f);
}

void Scene::Clean() {
	glDeleteBuffers(2, this->vboID);
	glDeleteVertexArrays(1, &this->vaoID);

	if (this->camera != nullptr) {
		delete this->camera;
		this->camera = nullptr;
	}
}
