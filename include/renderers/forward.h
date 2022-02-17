#ifndef RENDERERS_FORWARD_H
#define RENDERERS_FORWARD_H

#include "renderers/renderer.h"

#include "shader.h"

class ForwardRenderer: public Renderer
{
public:
	ForwardRenderer();
	~ForwardRenderer();

	void Init();

	void Render();

private:
	Shader shader;
};

#endif // RENDERERS_FORWARD_H
