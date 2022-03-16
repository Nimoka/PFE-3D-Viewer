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
	this->CleanShaders();

	if (this->directionalLightsDirection != nullptr)
		delete this->directionalLightsDirection;
	if (this->directionalLightsIntensity != nullptr)
		delete this->directionalLightsIntensity;
	if (this->pointLightsPosition != nullptr)
		delete this->pointLightsPosition;
	if (this->pointLightsIntensity != nullptr)
		delete this->pointLightsIntensity;
}

void ForwardRenderer::Init() {
	if (this->scene != nullptr)
		this->scene->SetRenderer(this);
	this->SetFullPassRender();
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

	this->shaders[0]->Activate();

	glUniformMatrix4fv(
			this->shaders[0]->GetUniformLocation("projection_matrix"), 1, false,
			this->scene->GetCamera()->ComputeProjectionMatrix().data());
	glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("model_matrix"), 1,
			false, this->scene->GetMeshTransformationMatrix().data());
	glUniformMatrix3fv(this->shaders[0]->GetUniformLocation("normal_matrix"), 1,
			false, this->scene->GetNormalMatrix().data());
	if (this->scene->navigate3D){
		glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("view_matrix"),
				1, false,
				this->scene->GetCamera()->Compute3DViewMatrix().data());
	} else {
		glUniformMatrix4fv(this->shaders[0]->GetUniformLocation("view_matrix"),
				1, false, this->scene->GetCamera()->ComputeViewMatrix().data());
	}
	glUniform3fv(this->shaders[0]->GetUniformLocation("lights_dir_direction"),
			this->nbDirectionalLights, this->directionalLightsDirection);
	glUniform3fv(this->shaders[0]->GetUniformLocation("lights_dir_intensity"),
			this->nbDirectionalLights, this->directionalLightsIntensity);
	glUniform3fv(this->shaders[0]->GetUniformLocation("lights_pt_position"),
			this->nbPointLights, this->pointLightsPosition);
	glUniform3fv(this->shaders[0]->GetUniformLocation("lights_pt_intensity"),
			this->nbPointLights, this->pointLightsIntensity);
	glUniform3fv(this->shaders[0]->GetUniformLocation("ambient_color"), 1,
			this->scene->GetAmbientColor().data());

	this->scene->RenderMesh(this->shaders[0]);

	this->shaders[0]->Deactivate();

	this->DeactivateContext();
}

void ForwardRenderer::UpdateDirectionalLightList(bool reload) {
	if (this->scene == nullptr)
		return;
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
		for (unsigned int i = 0; i < this->nbShaders; i++) {
			if (this->shaders[i] != nullptr) {
				this->shaders[i]->SetPreProcessorMacro(SPPM_NB_DIR_LIGHTS,
						std::to_string(this->nbDirectionalLights));
			}
		}
		if (reload)
			this->ReloadShaders();
	}
}

void ForwardRenderer::UpdatePointLightList(bool reload) {
	if (this->scene == nullptr)
		return;
	if (this->pointLightsPosition != nullptr)
		delete this->pointLightsPosition;
	if (this->pointLightsIntensity != nullptr)
		delete this->pointLightsIntensity;

	std::vector<PointLight*>* lights = this->scene->GetPointLights();
	this->nbPointLights = lights->size();
	this->pointLightsPosition =
			(float*) malloc(sizeof(float) * 3 * nbPointLights);
	this->pointLightsIntensity =
			(float*) malloc(sizeof(float) * 3 * nbPointLights);

	PointLight* light;
	for (unsigned int i = 0; i < this->nbPointLights; i++){
		light = lights->at(i);
		this->pointLightsPosition[3 * i] = light->GetPosition()[0];
		this->pointLightsPosition[3 * i + 1] = light->GetPosition()[1];
		this->pointLightsPosition[3 * i + 2] = light->GetPosition()[2];
		this->pointLightsIntensity[3 * i] = light->GetIntensity()[0];
		this->pointLightsIntensity[3 * i + 1]  = light->GetIntensity()[1];
		this->pointLightsIntensity[3 * i + 2]  = light->GetIntensity()[2];
	}

	if (this->shaders != nullptr) {
		for (unsigned int i = 0; i < this->nbShaders; i++) {
			if (this->shaders[i] != nullptr) {
				this->shaders[i]->SetPreProcessorMacro(SPPM_NB_PT_LIGHTS,
						std::to_string(this->nbPointLights));
			}
		}
		if (reload)
			this->ReloadShaders();
	}
}

void ForwardRenderer::SetFullPassRender() {
	if (this->scene == nullptr)
		return;

	this->CleanShaders();

	this->shaders = (ShadersReader**) malloc(sizeof(void*));
	this->nbShaders = 1;

	this->shaders[0] = new ShadersReader(this->context);
	this->UpdateDirectionalLightList(false);
	this->UpdatePointLightList(false);
	this->shaders[0]->LoadFiles(
			DATA_DIR "shaders/forward.vert",
			DATA_DIR "shaders/forward.frag",
			false);
}

void ForwardRenderer::SetPerMaterialRender() {
	if (this->scene == nullptr)
		return;
	if (this->scene->GetMesh())
		return;

	this->CleanShaders();

	this->nbShaders = this->scene->GetMesh()->nbMaterials;
	this->shaders = (ShadersReader**) malloc(sizeof(void*) * this->nbShaders);

	for (unsigned char i = 0; i < this->nbShaders; i++)
		this->shaders[i] = new ShadersReader(this->context);

	this->UpdateDirectionalLightList(false);
	this->UpdatePointLightList(false);

	for (unsigned char i = 0; i < this->nbShaders; i++) {
		this->shaders[i]->LoadFiles(
				DATA_DIR "shaders/forward.vert",
				DATA_DIR "shaders/forward.frag",
				false);
	}
}

void ForwardRenderer::SetScene(Scene* scene) {
	this->scene = scene;
	if (this->scene != nullptr) {
		this->scene->SetRenderer(this);
		this->UpdateDirectionalLightList(false);
		this->UpdatePointLightList();
	}
}
