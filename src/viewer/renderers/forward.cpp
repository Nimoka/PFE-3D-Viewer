#include "renderers/forward.h"

#include <string>

#include "light.h"
#include "shadersreader.h"

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
	if (this->shaders != nullptr)
		delete this->shaders;
}

void ForwardRenderer::Init() {
	if (this->scene != nullptr)
		this->scene->SetRenderer(this);

	this->shaders = new ShadersReader(this->context);
	this->UpdateDirectionalLightList();
	this->shaders->LoadFiles(
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

	this->shaders->Activate();

	glUniformMatrix4fv(this->shaders->GetUniformLocation("projection_matrix"),
			1, false,
			this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shaders->GetUniformLocation("view_matrix"), 1,
			false, this->scene->GetCamera()->ComputeViewMatrix().data());
	glUniformMatrix4fv(this->shaders->GetUniformLocation("model_matrix"), 1,
			false, this->scene->GetMeshTransformationMatrix().data());
	glUniformMatrix3fv(this->shaders->GetUniformLocation("normal_matrix"), 1,
			false, this->scene->GetNormalMatrix().data());

	glUniform3fv(this->shaders->GetUniformLocation("lights_dir_direction"),
			this->nbDirectionalLights, this->directionalLightsDirection);
	glUniform3fv(this->shaders->GetUniformLocation("lights_dir_intensity"),
			this->nbDirectionalLights, this->directionalLightsIntensity);

	glUniform3fv(this->shaders->GetUniformLocation("ambient_color"), 1,
			this->scene->GetAmbientColor().data());

	this->scene->RenderMesh(this->shaders);

	this->shaders->Deactivate();

	this->DeactivateContext();
}

void ForwardRenderer::UpdateDirectionalLightList() {
	if (this->directionalLightsDirection != nullptr)
		delete this->directionalLightsDirection;
	if (this->directionalLightsDirection != nullptr)
		delete this->directionalLightsIntensity;

	std::vector<DirectionalLight*>* lights =
			this->scene->GetDirectionalLights();
	this->nbDirectionalLights = lights->size();
	this->directionalLightsDirection =
			(float*) malloc(sizeof(float) * 3 * nbDirectionalLights);
	this->directionalLightsIntensity =
			(float*) malloc(sizeof(float) * 3 * nbDirectionalLights);

	DirectionalLight* light;
	for (unsigned int i = 0; i < this->nbDirectionalLights; i++) {
		light = lights->at(i);
		this->directionalLightsDirection[3 * i] = light->GetDirection()[0];
		this->directionalLightsDirection[3 * i + 1] = light->GetDirection()[1];
		this->directionalLightsDirection[3 * i + 2] = light->GetDirection()[2];
		this->directionalLightsIntensity[3 * i] = light->GetIntensity()[0];
		this->directionalLightsIntensity[3 * i + 1] = light->GetIntensity()[1];
		this->directionalLightsIntensity[3 * i + 2] = light->GetIntensity()[2];
	}

	if (this->shaders != nullptr) {
		this->shaders->SetPreProcessorMacro(SPPM_NB_DIR_LIGHTS,
				std::to_string(this->nbDirectionalLights));
		this->shaders->Load();
	}
}

void ForwardRenderer::SetScene(Scene* scene) {
	this->scene = scene;
	if (this->scene != nullptr)
		this->scene->SetRenderer(this);
}
