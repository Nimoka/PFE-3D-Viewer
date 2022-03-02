#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderers/renderer.h"

#include "shaderreader.h"

class ForwardRenderer: public Renderer
{
public:
	ForwardRenderer(void* context);
	ForwardRenderer(void* context, Scene* scene);
	ForwardRenderer(Renderer* renderer);
	~ForwardRenderer();

	void Init();

	void Render(ImVec2 size);

	void ReloadShaders();

private:
	ShaderReader* shader = nullptr;
};

#endif // RENDERER_FORWARD_H
