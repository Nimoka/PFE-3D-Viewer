#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include <imgui.h>

#include "scene.h"
#include "shadersreader.h"

class Renderer
{
public:
	Renderer(void* context, bool renderingPerMaterial);
	Renderer(void* context, Scene* scene, bool renderingPerMaterial);
	Renderer(Renderer* renderer);
	virtual ~Renderer();

	virtual void Render(ImVec2 size) = 0;

	virtual void UpdateDirectionalLightList(bool reload = true) = 0;
	virtual void UpdatePointLightList(bool reload = true) = 0;
	void ReloadShaders();

	virtual void SetFullPassRender() = 0;
	virtual void SetPerMaterialRender() = 0;

	void* GetContext();
	const Eigen::Vector4f& GetClearColor();
	const GLuint& GetRenderTexture() const;
	Scene* GetScene();
	unsigned char GetNbShaders();
	ShadersReader** GetShaders();
	bool IsRenderingPerMaterial();

	void SetClearColor(Eigen::Vector4f color);
	void SetRenderingPerMaterial(bool value);
	void SetScene(Scene* scene);

protected:
	void ActivateContext();
	const void DeactivateContext();

	void InitScene();

	void CleanShaders();

	void* context = nullptr;

	Eigen::Vector4f clearColor = Eigen::Vector4f(0., 0., 0., 1.);
	Scene* scene = nullptr;

	bool renderingPerMaterial = false;

	unsigned char nbShaders = 0;
	ShadersReader** shaders = nullptr;

private:
	void Init();
	void Clean();

	GLuint renderFboID;
	GLuint renderRboID;
	GLuint renderTextureID;
};

#endif // RENDERERS_RENDERER_H
