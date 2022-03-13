#include "scene.h"

#include "renderers/renderer.h"

Scene::Scene()
		: camera(new Camera()) {
	this->AddDirectionalLight(
			new DirectionalLight(
					Eigen::Vector3f(.2, .2, .2),
					Eigen::Vector3f(1., 1., 0.)));
	this->AddPointLight(new PointLight(Eigen::Vector3f(.1,.1,.1)));
}

Scene::Scene(Mesh* mesh) {
	this->mesh = mesh;
	this->Init();
}

Scene::~Scene() {
	for (auto i: this->directionalLights)
		delete i;

	for (auto i : this->pointLights)
		delete i;

	this->Clean();
}

bool Scene::RenderMesh(ShadersReader* shaders) {
	if (this->mesh == nullptr)
		return false;

	glBindVertexArray(this->vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID[1]);

	int vertexLocation = shaders->GetAttribLocation("vtx_position");
	if (vertexLocation >= 0) {
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) 0));
		glEnableVertexAttribArray(vertexLocation);
	}

	int colorLocation = shaders->GetAttribLocation("vtx_color");
	if (colorLocation >= 0) {
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) sizeof(Eigen::Vector3f)));
		glEnableVertexAttribArray(colorLocation);
	}

	int normalLocation = shaders->GetAttribLocation("vtx_normal");
	if (normalLocation >= 0) {
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), ((void*) (2 * sizeof(Eigen::Vector3f))));
		glEnableVertexAttribArray(normalLocation);
	}

	int materialTexLocation = shaders->GetUniformLocation("face_material");
	if (materialTexLocation >= 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, this->tboMaterialsTex);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, this->tboMaterialsID);
		glUniform1i(materialTexLocation, 0);
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

void Scene::AddDirectionalLight(DirectionalLight* light) {
	if (light == nullptr)
		return;

	this->directionalLights.push_back(light);

	if (this->renderer != nullptr)
		((Renderer*) this->renderer)->UpdateDirectionalLightList();
}

void Scene::AddPointLight(PointLight *light){
	if (light == nullptr)
		return;

	this->pointLights.push_back(light);

	if (this->renderer != nullptr)
		((Renderer *)this->renderer)->UpdatePointLightList();
 }

const Eigen::Vector3f& Scene::GetAmbientColor() {
	return this->ambientColor;
}

Camera* Scene::GetCamera() {
	return this->camera;
}

std::vector<DirectionalLight*>* Scene::GetDirectionalLights() {
	return &this->directionalLights;
}

std::vector<PointLight*>* Scene::GetPointLights(){
  return &this->pointLights;
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

void Scene::SetMeshTransformationMatrix(Eigen::Matrix4f transformationMatrix) {
	this->meshTransformationMatrix = transformationMatrix;
}

void Scene::SetRenderer(void* renderer) {
	this->renderer = renderer;
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

	glGenBuffers(1, &this->tboMaterialsID);
	glBindBuffer(GL_TEXTURE_BUFFER, this->tboMaterialsID);
	glBufferData(GL_TEXTURE_BUFFER, ((this->mesh->nbFaces) * sizeof(char)),
			this->mesh->facesMaterials, GL_STATIC_DRAW);
	glGenTextures(1, &this->tboMaterialsTex);

	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

void Scene::Clean() {
	glDeleteBuffers(2, this->vboID);
	glDeleteVertexArrays(1, &this->vaoID);

	if (this->camera != nullptr) {
		delete this->camera;
		this->camera = nullptr;
	}
}
