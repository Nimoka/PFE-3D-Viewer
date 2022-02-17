#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include "scene.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {}

	virtual void Render() = 0;

	Scene* GetScene();
	GLuint GetFboID();
	GLuint GetTextureID();
	GLuint* GetTextureIDPointer();

	void SetScene(Scene* scene);

protected:
	Scene* scene;

	GLuint fboID;
	GLuint textureID;
};

#endif // RENDERERS_RENDERER_H
