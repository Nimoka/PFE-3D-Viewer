#ifndef RENDERERS_SIMPLE_H
#define RENDERERS_SIMPLE_H

#include "renderers/renderer.h"

class SimpleRenderer: public Renderer
{
public:
	SimpleRenderer(void* context);
	SimpleRenderer(void* context, Scene* scene);
	SimpleRenderer(Renderer* renderer);
	~SimpleRenderer();

	void Init();

	void Render(ImVec2 size);

	void UpdateDirectionalLightList();
};

#endif // RENDERERS_SIMPLE_H
