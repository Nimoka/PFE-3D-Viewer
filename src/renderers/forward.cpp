#include "renderers/forward.h"

ForwardRenderer::ForwardRenderer()
: Renderer() {
	this->Init();
}

ForwardRenderer::~ForwardRenderer() {
	if (this->shader != nullptr)
		delete this->shader;
}

void ForwardRenderer::Init() {
	this->shader = new ShaderReader(DATA_DIR "shaders/simple.vert",
			DATA_DIR "shaders/simple.frag", false);
}

void ForwardRenderer::Render(ImVec2 size) {
	this->ActivateContext();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glViewport(0, 0, size.x, size.y);

	// glClearColor(.8, .8, .8, 1.);
	glClearColor(.2, .2, .2, 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shader->Activate();

	glUniformMatrix4fv(this->shader->GetUniformLocation("projection_matrix"), 1,
			false, this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shader->GetUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->ComputeViewMatrix().data());

	this->scene->Render(this->shader, size);

	this->shader->Deactivate();

	this->DeactivateContext();
}
