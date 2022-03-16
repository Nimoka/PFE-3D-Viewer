#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderers/renderer.h"

class ForwardRenderer: public Renderer
{
public:
	ForwardRenderer(void* context);
	ForwardRenderer(void* context, Scene* scene);
	ForwardRenderer(Renderer* renderer);
	~ForwardRenderer();

	void Init();

	void Render(ImVec2 size);

	void UpdateDirectionalLightList(bool reload = true);
	void UpdatePointLightList(bool reload = true);

	void SetFullPassRender();
	void SetPerMaterialRender();

	void SetScene(Scene* scene);

private:
	// Directional lights data
	unsigned int nbDirectionalLights = 0;
	float* directionalLightsDirection = nullptr;
	float* directionalLightsIntensity = nullptr;

	// Point lights data
	unsigned int nbPointLights = 0;
	float* pointLightsPosition = nullptr;
	float* pointLightsIntensity = nullptr;
};

#endif // RENDERER_FORWARD_H
