#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H



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

	Scene* scene;
};

#endif // RENDERERS_RENDERER_H
