#include "renderers/forward.h"

ForwardRenderer::ForwardRenderer()
: Renderer() {
	this->Init();
}

ForwardRenderer::~ForwardRenderer() {}

void ForwardRenderer::Init() {
	shader.loadFromFiles(DATA_DIR "shaders/simple.vert", DATA_DIR "shaders/simple.frag");
}

void ForwardRenderer::Render() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(.5, .5, .5, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
