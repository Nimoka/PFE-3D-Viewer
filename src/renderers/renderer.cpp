#include "renderers/renderer.h"

#include <iostream>

Renderer::Renderer() {
	/* Create render FBO */

	glbinding::aux::enableGetErrorCallback();

	// Create a frame buffer
	glGenFramebuffers(1, &this->fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	// Create a texture
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Set pool filtering to nearest (should be pixel perfect on screen)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create a depth buffer
	glGenRenderbuffers(1, &this->rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 800);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rboID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer initialization failed!" << std::endl;

	// Switch back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Scene* Renderer::GetScene() {
	return this->scene;
}

const GLuint& Renderer::GetFboID() const {
	return this->fboID;
}

const GLuint& Renderer::GetTextureID() const {
	return this->textureID;
}

void Renderer::SetScene(Scene* scene) {
	this->scene = scene;
}
