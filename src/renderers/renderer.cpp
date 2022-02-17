#include "renderers/renderer.h"

#include <iostream>

Renderer::Renderer() {
	std::cout << "Here" << std::endl;

	/* Create render FBO */

	glbinding::aux::enableGetErrorCallback();

	// Create a framebuffer
	glGenFramebuffers(1, &this->fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboID);

	// Create the render texture
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0);

	// Set the list of attached buffers
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureID, 0);
	GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	// Return to first framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Scene* Renderer::GetScene() {
	return this->scene;
}

GLuint Renderer::GetFboID() {
	return this->fboID;
}

GLuint Renderer::GetTextureID() {
	return this->textureID;
}

GLuint* Renderer::GetTextureIDPointer() {
	return &this->textureID;
}

void Renderer::SetScene(Scene* scene) {
	this->scene = scene;
}
