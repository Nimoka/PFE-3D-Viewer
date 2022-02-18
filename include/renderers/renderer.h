#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include "imgui.h"

#include "scene.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {}

	virtual void Render(ImVec2 size) = 0;

	Scene* GetScene();
	const GLuint& GetFboID() const;
	const GLuint& GetTextureID() const;

	void SetScene(Scene* scene);

protected:
	Scene* scene;

	GLuint fboID;
	GLuint rboID;
	GLuint textureID;
};

#endif // RENDERERS_RENDERER_H
