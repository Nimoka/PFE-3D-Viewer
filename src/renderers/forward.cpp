#include "renderers/forward.h"

ForwardRenderer::ForwardRenderer()
: Renderer() {
	this->Init();
}

ForwardRenderer::~ForwardRenderer() {}

void ForwardRenderer::Init() {
	this->shader.loadFromFiles(DATA_DIR "shaders/simple.vert",
			DATA_DIR "shaders/simple.frag");
}

void ForwardRenderer::Render(ImVec2 size) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glViewport(0, 0, size.x, size.y);

	glClearColor(.1, .1, .5, 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shader.activate();

	glUniformMatrix4fv(this->shader.getUniformLocation("projection_matrix"), 1,
			false, this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shader.getUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->ComputeViewMatrix().data());

	this->scene->Render(&this->shader, size);

	this->shader.deactivate();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
