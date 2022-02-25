#include "renderers/renderer.h"

#include <iostream>

Renderer::Renderer() {
	/* Create render FBO */

	// Create a frame buffer
	glGenFramebuffers(1, &this->renderFboID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderFboID);

	// Create a texture
	glGenTextures(1, &this->renderTextureID);
	glBindTexture(GL_TEXTURE_2D, this->renderTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			this->renderTextureID, 0);

	// Set pool filtering to nearest (should be pixel perfect on screen)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Create a depth buffer
	glGenRenderbuffers(1, &this->renderRboID);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderRboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 128, 128);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, this->renderRboID);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer initialization failed!" << std::endl;

	// Switch back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Eigen::Vector4f& Renderer::GetClearColor() {
	return this->clearColor;
}

Scene* Renderer::GetScene() {
	return this->scene;
}

const GLuint& Renderer::GetRenderTexture() const {
	return this->renderTextureID;
}

void Renderer::SetClearColor(Eigen::Vector4f color) {
	this->clearColor = color;
}

void Renderer::SetScene(Scene* scene) {
	if (this->scene != nullptr)
		delete this->scene;
	this->scene = scene;
}

void Renderer::ActivateContext() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderFboID);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderRboID);
	glBindTexture(GL_TEXTURE_2D, this->renderTextureID);
}

const void Renderer::DeactivateContext() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
