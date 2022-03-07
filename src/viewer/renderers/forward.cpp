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
	glUniformMatrix4fv(this->shader->GetUniformLocation("model_matrix"), 1,
			false, this->scene->GetMeshTransformationMatrix().data());
	glUniformMatrix3fv(this->shader->GetUniformLocation("normal_matrix"), 1,
			false, this->scene->GetNormalMatrix().data());

	{
		unsigned int nbDirectionalLights;
		float* directionalLightsDirection;
		float* directionalLightsIntensity;

		std::vector<DirectionalLight*>* lights = this->scene->GetLights();
		nbDirectionalLights = lights->size();
		directionalLightsDirection =
				(float*) malloc(sizeof(float) * 3 * nbDirectionalLights);
		directionalLightsIntensity =
				(float*) malloc(sizeof(float) * 3 * nbDirectionalLights);

		DirectionalLight* light;
		for (unsigned int i = 0; i < nbDirectionalLights; i++) {
			light = lights->at(i);
			directionalLightsDirection[3 * i] = light->GetDirection()[0];
			directionalLightsDirection[3 * i + 1] = light->GetDirection()[1];
			directionalLightsDirection[3 * i + 2] = light->GetDirection()[2];
			directionalLightsIntensity[3 * i] = light->GetIntensity()[0];
			directionalLightsIntensity[3 * i + 1] = light->GetIntensity()[1];
			directionalLightsIntensity[3 * i + 2] = light->GetIntensity()[2];
		}

		glUniform1i(this->shader->GetUniformLocation("lights_dir_nb"),
				nbDirectionalLights);
		glUniform3fv(this->shader->GetUniformLocation("lights_dir_direction"),
				nbDirectionalLights, directionalLightsDirection);
		glUniform3fv(this->shader->GetUniformLocation("lights_dir_intensity"),
				nbDirectionalLights, directionalLightsIntensity);

		delete directionalLightsDirection;
		delete directionalLightsIntensity;
	}

	glUniform3fv(this->shader->GetUniformLocation("ambient_color"), 1,
			this->scene->GetAmbientColor().data());

	this->scene->RenderMesh(this->shader);

	this->shader->Deactivate();

	this->DeactivateContext();
}
