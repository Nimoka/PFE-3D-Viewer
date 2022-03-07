#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include <imgui.h>

#include "scene.h"
#include "shaderreader.h"

class Renderer
{
public:
	Renderer(void* context);
	Renderer(void* context, Scene* scene);
	Renderer(Renderer* renderer);
	virtual ~Renderer();

	virtual void Render(ImVec2 size) = 0;

	void ReloadShaders();

	void* GetContext();
	const Eigen::Vector4f& GetClearColor();
	const GLuint& GetRenderTexture() const;
	Scene* GetScene();
	ShaderReader* GetShader();

	void SetClearColor(Eigen::Vector4f color);
	void SetScene(Scene* scene);

protected:
	void ActivateContext();
	const void DeactivateContext();

	void* context = nullptr;

	Eigen::Vector4f clearColor = Eigen::Vector4f(0., 0., 0., 1.);
	Scene* scene = nullptr;

	ShaderReader* shader = nullptr;

private:
	void Init();
	void Clean();

	GLuint renderFboID;
	GLuint renderRboID;
	GLuint renderTextureID;
};

#endif // RENDERERS_RENDERER_H
