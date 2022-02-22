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

	virtual void ReloadShaders() = 0;

	Scene* GetScene();
	const GLuint& GetRenderTexture() const;

	void SetScene(Scene* scene);

protected:
	void ActivateContext();
	const void DeactivateContext();

	Scene* scene;

private:
	GLuint renderFboID;
	GLuint renderRboID;
	GLuint renderTextureID;
};

#endif // RENDERERS_RENDERER_H
