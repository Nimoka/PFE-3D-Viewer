#include "renderers/forward.h"

ForwardRenderer::ForwardRenderer()
: Renderer() {
	this->Init();
}

ForwardRenderer::~ForwardRenderer() {}

void ForwardRenderer::Init() {
	shader.loadFromFiles(DATA_DIR "shaders/simple.vert", DATA_DIR "shaders/simple.frag");
}

void ForwardRenderer::Render(ImVec2 size) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glViewport(0, 0, size.x, size.y);

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0., 0., 1., 1.);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
