#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include <globjects/Framebuffer.h>

#include "scene.h"

class Renderer
{
public:
	Renderer() {}
	virtual ~Renderer() {}

	virtual void Render() = 0;

	Scene* GetScene();


	void SetScene(Scene* scene);

protected:
	globjects::Framebuffer* framebuffer;
	Scene* scene;
};

#endif // RENDERERS_RENDERER_H
