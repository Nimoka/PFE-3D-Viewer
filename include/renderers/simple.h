#ifndef RENDERERS_SIMPLE_H
#define RENDERERS_SIMPLE_H

#include "renderers/renderer.h"

#include "shaderreader.h"

class SimpleRenderer: public Renderer
{
public:
	SimpleRenderer();
	~SimpleRenderer();

	void Init();

	void Render(ImVec2 size);

	void ReloadShaders();

private:
	ShaderReader* shader;
};

#endif // RENDERERS_SIMPLE_H
