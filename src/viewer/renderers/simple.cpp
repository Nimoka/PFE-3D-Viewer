#include "renderers/simple.h"

#include <iostream>

SimpleRenderer::SimpleRenderer(void* context, bool renderingPerMaterial)
		: Renderer(context, renderingPerMaterial) {}

SimpleRenderer::SimpleRenderer(void* context, Scene* scene,
		bool renderingPerMaterial)
		: Renderer(context, scene, renderingPerMaterial) {
	this->Init();
}

SimpleRenderer::SimpleRenderer(Renderer* renderer)
		: Renderer(renderer) {
	this->Init();
}

SimpleRenderer::~SimpleRenderer() {
	this->CleanShaders();
}

void SimpleRenderer::Init() {
	if (this->scene != nullptr)
		this->scene->SetRenderer(nullptr);
	this->SetFullPassRender();
}

void SimpleRenderer::Render(ImVec2 size) {
	this->ActivateContext();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glViewport(0, 0, size.x, size.y);
	this->scene->UpdateCameraViewport(size);

	glClearColor(this->clearColor[0], this->clearColor[1], this->clearColor[2],
			this->clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shaders[0]->Activate();

	glUniformMatrix4fv(
			this->shaders[0]->GetUniformLocation("projection_matrix"), 1, false,
			this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("model_matrix"), 1,
			false, this->scene->GetMeshTransformationMatrix().data());
	glUniformMatrix3fv(this->shaders[0]->GetUniformLocation("normal_matrix"), 1,
			false, this->scene->GetNormalMatrix().data());
	if (this->scene->navigate3D) {
		glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("view_matrix"),
				1, false,
				this->scene->GetCamera()->Compute3DViewMatrix().data());
	} else {
		glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("view_matrix"),
				1, false, this->scene->GetCamera()->ComputeViewMatrix().data());
	}

	this->scene->RenderMesh(this->shaders[0]);

	this->shaders[0]->Deactivate();

	this->DeactivateContext();
}

void SimpleRenderer::UpdateDirectionalLightList(bool reload) {}
void SimpleRenderer::UpdatePointLightList(bool reload) {}

void SimpleRenderer::SetFullPassRender() {
	this->CleanShaders();

	this->shaders = (ShadersReader**) malloc(sizeof(void*));
	this->nbShaders = 1;

	this->shaders[0] = new ShadersReader(this->context,
			DATA_DIR "shaders/simple.vert",
			DATA_DIR "shaders/simple.frag",
			false);
}

void SimpleRenderer::SetPerMaterialRender() {
	if (this->scene == nullptr)
		return;
	Mesh* mesh = this->scene->GetMesh();
	if (mesh == nullptr)
		return;

	this->CleanShaders();

	this->nbShaders = this->scene->GetMesh()->nbMaterials;
	this->shaders = (ShadersReader**) malloc(sizeof(void*) * this->nbShaders);

	for (unsigned char i = 0; i < this->nbShaders; i++) {
		this->shaders[i] = new ShadersReader(this->context,
			DATA_DIR "shaders/simple.vert",
			DATA_DIR "shaders/simple.frag",
			false);
	}
}
