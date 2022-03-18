#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderers/renderer.h"

class ForwardRenderer: public Renderer
{
public:
	ForwardRenderer(void* context, bool renderingPerMaterial = false);
	ForwardRenderer(void* context, Scene* scene,
			bool renderingPerMaterial = false);
	ForwardRenderer(Renderer* renderer);
	~ForwardRenderer();

	void Render(ImVec2 size);

	void UpdateDirectionalLightList(bool reload = true);
	void UpdatePointLightList(bool reload = true);

	void SetScene(Scene* scene);

protected:
	void InitFullPassShaders();
	void InitPerMaterialShaders();

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
