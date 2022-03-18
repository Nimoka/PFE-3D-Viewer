#ifndef RENDERERS_SIMPLE_H
#define RENDERERS_SIMPLE_H

#include "renderers/renderer.h"

class SimpleRenderer: public Renderer
{
public:
	SimpleRenderer(void* context, bool renderingPerMaterial = false);
	SimpleRenderer(void* context, Scene* scene,
			bool renderingPerMaterial = false);
	SimpleRenderer(Renderer* renderer);
	~SimpleRenderer();

	void Render(ImVec2 size);

	void UpdateDirectionalLightList(bool reload = true);
	void UpdatePointLightList(bool reload = true);

	void SetFullPassRender();
	void SetPerMaterialRender();
};

#endif // RENDERERS_SIMPLE_H
