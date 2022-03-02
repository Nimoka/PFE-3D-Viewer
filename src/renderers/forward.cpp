#include "renderers/forward.h"

#include "light.h"

ForwardRenderer::ForwardRenderer(void* context)
		: Renderer(context) {
	this->Init();
}

ForwardRenderer::ForwardRenderer(void* context, Scene* scene)
		: Renderer(context, scene) {
	this->Init();
}

ForwardRenderer::ForwardRenderer(Renderer* renderer)
		: Renderer(renderer) {
	this->Init();
}

ForwardRenderer::~ForwardRenderer() {
	if (this->shader != nullptr)
		delete this->shader;
}

void ForwardRenderer::Init() {
	this->shader = new ShaderReader(this->context,
			DATA_DIR "shaders/forward.vert",
			DATA_DIR "shaders/forward.frag",
			false);
}

void ForwardRenderer::Render(ImVec2 size) {
	this->ActivateContext();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 0);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glBlendFunc(GL_ONE, GL_ONE);

	glViewport(0, 0, size.x, size.y);
	this->scene->UpdateCameraViewport(size);

	glClearColor(this->clearColor[0], this->clearColor[1], this->clearColor[2],
			this->clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shader->Activate();

	glUniformMatrix4fv(this->shader->GetUniformLocation("projection_matrix"), 1,
			false, this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shader->GetUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->ComputeViewMatrix().data());

	std::vector<Light*>* lights = this->scene->GetLights();
	auto it = lights->begin();
	if (it != lights->end()) {
		(*it)->Activate(this->shader);
		this->scene->RenderMesh(this->shader);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		for (it++; it != lights->end(); it++) {
			(*it)->Activate(this->shader);
			this->scene->RenderMesh(this->shader);
		}
		glDisable(GL_BLEND);
	}

	this->shader->Deactivate();

	this->DeactivateContext();
}

void ForwardRenderer::ReloadShaders() {
	if (this->shader != nullptr)
		this->shader->Load();
}
