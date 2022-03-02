#include "renderers/simple.h"
#include <iostream>

SimpleRenderer::SimpleRenderer(void* context)
		: Renderer(context) {
	this->Init();
}

SimpleRenderer::SimpleRenderer(void* context, Scene* scene)
		: Renderer(context, scene) {
	this->Init();
}

SimpleRenderer::SimpleRenderer(Renderer* renderer)
		: Renderer(renderer) {
	this->Init();
}

SimpleRenderer::~SimpleRenderer() {
	if (this->shader != nullptr)
		delete this->shader;
}

void SimpleRenderer::Init() {
	this->shader = new ShaderReader(this->context,
			DATA_DIR "shaders/simple.vert",
			DATA_DIR "shaders/simple.frag",
			false);
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

	this->shader->Activate();

	glUniformMatrix4fv(this->shader->GetUniformLocation("projection_matrix"), 1,
			false, this->scene->GetCamera()->ComputeProjectionMatrix().data());
	if(!this->scene->navigate3D){
		glUniformMatrix4fv(this->shader->GetUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->ComputeViewMatrix().data());
	}else{
		glUniformMatrix4fv(this->shader->GetUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->Compute3DViewMatrix().data());
	}	

	this->scene->RenderMesh(this->shader);

	this->shader->Deactivate();

	this->DeactivateContext();
}

void SimpleRenderer::ReloadShaders() {
	if (this->shader != nullptr)
		this->shader->Load();
}
