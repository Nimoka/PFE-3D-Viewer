#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include "imgui.h"

#include "scene.h"

class Renderer
{
public:
	Renderer();
	Renderer(Scene* scene);
	Renderer(Renderer* renderer);
	virtual ~Renderer();

	virtual void Render(ImVec2 size) = 0;

	virtual void ReloadShaders() = 0;

	const Eigen::Vector4f& GetClearColor();
	Scene* GetScene();
	const GLuint& GetRenderTexture() const;

	void SetClearColor(Eigen::Vector4f color);
	void SetScene(Scene* scene);

protected:
	void ActivateContext();
	const void DeactivateContext();

	Eigen::Vector4f clearColor = Eigen::Vector4f(0., 0., 0., 1.);
	Scene* scene = nullptr;

private:
	void Init();
	void Clean();

	GLuint renderFboID;
	GLuint renderRboID;
	GLuint renderTextureID;
};

#endif // RENDERERS_RENDERER_H
