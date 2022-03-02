#ifndef RENDERERS_SIMPLE_H
#define RENDERERS_SIMPLE_H

#include "renderers/renderer.h"

#include "shaderreader.h"

class SimpleRenderer: public Renderer
{
public:
	SimpleRenderer(void* context);
	SimpleRenderer(void* context, Scene* scene);
	SimpleRenderer(Renderer* renderer);
	~SimpleRenderer();

	void Init();

	void Render(ImVec2 size);

	void ReloadShaders();

private:
	ShaderReader* shader = nullptr;
};

#endif // RENDERERS_SIMPLE_H
