#include "renderers/renderer.h"

#include <iostream>

Renderer::Renderer(void* context, bool renderingPerMaterial)
		: context(context)
		, renderingPerMaterial(renderingPerMaterial) {
	this->Init();
}

Renderer::Renderer(void* context, Scene* scene, bool renderingPerMaterial)
		: context(context)
		, scene(scene)
		, renderingPerMaterial(renderingPerMaterial) {
	this->Init();
}

Renderer::Renderer(Renderer* renderer)
		: context(renderer->GetContext())
		, scene(renderer->GetScene())
		, clearColor(renderer->GetClearColor())
		, renderingPerMaterial(renderer->IsRenderingPerMaterial()) {
	this->Init();
}

void Renderer::InitShaders(bool updateVbos) {
	if (this->renderingPerMaterial)
		this->InitPerMaterialShaders();
	else
		this->InitFullPassShaders();
	if ((this->scene != nullptr) && updateVbos)
		this->scene->UpdateVbos();
}

void Renderer::ReloadShaders() {
	if (this->shaders != nullptr) {
		for (unsigned char i = 0; i < this->nbShaders; i++) {
			if (this->shaders[i] != nullptr)
				this->shaders[i]->Load();
		}
	}
}

Renderer::~Renderer() {
	this->Clean();
}

void* Renderer::GetContext() {
	return this->context;
}

const Eigen::Vector4f& Renderer::GetClearColor() {
	return this->clearColor;
}

const GLuint& Renderer::GetRenderTexture() const {
	return this->renderTextureID;
}

Scene* Renderer::GetScene() {
	return this->scene;
}

unsigned char Renderer::GetNbShaders() {
	return this->nbShaders;
}

ShadersReader** Renderer::GetShaders() {
	return this->shaders;
}

bool Renderer::IsRenderingPerMaterial() {
	return this->renderingPerMaterial;
}

void Renderer::SetClearColor(Eigen::Vector4f color) {
	this->clearColor = color;
}

void Renderer::SetRenderingPerMaterial(bool value) {
	if (this->renderingPerMaterial == value)
		return;
	this->renderingPerMaterial = value;
	this->InitShaders();
}

void Renderer::SetScene(Scene* scene) {
	this->scene = scene;
	this->InitScene();
}

void Renderer::ActivateContext() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->renderFboID);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderRboID);
	glBindTexture(GL_TEXTURE_2D, this->renderTextureID);
}

const void Renderer::DeactivateContext() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::InitScene() {
	if (this->scene != nullptr) {
		this->scene->SetRenderer(this);
		this->InitShaders();
		this->scene->UpdateVbos();
	}
}

void Renderer::InitFullPassShaders() {}
void Renderer::InitPerMaterialShaders() {}

void Renderer::CleanShaders() {
	if (this->shaders != nullptr) {
		for (unsigned char i = 0; i < this->nbShaders; i++) {
			if (this->shaders[i] != nullptr)
				delete this->shaders[i];
		}
	}
	delete this->shaders;
	this->shaders = nullptr;
	this->nbShaders = 0;
}

void Renderer::Init() {
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

void Renderer::Clean() {
	glDeleteTextures(1, &this->renderTextureID);
	glDeleteRenderbuffers(1, &this->renderRboID);
	glDeleteFramebuffers(1, &this->renderFboID);
}
