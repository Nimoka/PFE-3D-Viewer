#ifndef RENDERERS_RENDERER_H
#define RENDERERS_RENDERER_H

#include "opengl.h"

#include <imgui.h>

#include "scene.h"
#include "shadersreader.h"

/**
 * \brief Virtual renderer.
 * 
 * Set default renderer elements on the graphics card and prepare the texture
 * that will contains the result of the rendering.
 */
class Renderer
{
public:
	/**
	 * \brief Constructor with no scene.
	 * 
	 * `Renderer` constructor with no scene provided.
	 * The real renderer can’t render while it does not have a scene.
	 * 
	 * \param context Application context using this module.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach (`false`)
	 */
	Renderer(void* context, bool renderingPerMaterial);
	/**
	 * \brief Constructor with scene.
	 * 
	 * `Renderer` constructor with a `Scene` object provided.
	 * 
	 * \param context Application context using this module.
	 * \param scene Scene to render.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach (`false`)
	 */
	Renderer(void* context, Scene* scene, bool renderingPerMaterial);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `Renderer` constructor using an existing object that would be duplicated.
	 * It accepts all types of renderers, that way it is possible to switch
	 * from to another just by usign this constructor.
	 * Elements specific to this type of renderer will be initialized, even if
	 * `renderer` is already a `Renderer`.
	 * 
	 * \param renderer Renderer to duplicate.
	 */
	Renderer(Renderer* renderer);
	/**
	 * \brief Destructor.
	 * 
	 * Virtual `Renderer` destructor.
	 * It will deallocate objects also on the graphics card.
	 */
	virtual ~Renderer();

	/**
	 * \brief Render the scene.
	 * 
	 * Pure virtual function for the rendering function.
	 * Because the size of the window can change at any moment, the size of the
	 * texture generated is always updated using the `size` parameter.
	 * 
	 * \param size Size of the texture to render.
	 */
	virtual void Render(ImVec2 size) = 0;

	/**
	 * \brief Update the list of directional lights.
	 * 
	 * Update the list of directional lights as it will be passed to the
	 * shaders (uniform array). Because the number of lights is directly set in
	 * the code of the shaders, shaders should be reloaded.
	 * Pure virtual function.
	 * 
	 * \param reload Reload shaders (`true`, default) or not (`false`). Might
	 *      be useful when adding or removing multiple lights via multiple
	 *      calls of the adding or removing function.
	 */
	virtual void UpdateDirectionalLightList(bool reload = true) = 0;
	/**
	 * \brief Update the list of point lights.
	 * 
	 * Update the list of point lights as it will be passed to the shaders
	 * (uniform array). Because the number of lights is directly set in the
	 * code of the shaders, shaders should be reloaded.
	 * Pure virtual function.
	 * 
	 * \param reload Reload shaders (`true`, default) or not (`false`). Might
	 *      be useful when adding or removing multiple lights via multiple
	 *      calls of the adding or removing function.
	 */
	virtual void UpdatePointLightList(bool reload = true) = 0;

	/**
	 * \brief Launch shaders initialization and will adapt _OpenGL_ buffers.
	 * 
	 * Launch `InitPerMaterialShaders()` or `InitFullPassShaders()` depending on
	 * the value of `renderingPerMaterial`. Will also recreate faces buffers
	 * (without modifying vertice buffer) depending on `updateVbos` parameter.
	 * 
	 * \param updateVbos Recreate faces buffers (`true`, default) or not
	 *      (`false`) without modifying vertice buffer
	 */
	void InitShaders(bool updateVbos = true);
	/**
	 * \brief Reload all shaders.
	 * 
	 * Reload all shaders if there is any.
	 */
	void ReloadShaders();

	/**
	 * \brief Getter of `context`.
	 * 
	 * Return the value of the `context` field, a pointer corresponding to the
	 * application context that is using this module.
	 * 
	 * \return Value of the `context` field.
	 */
	void* GetContext();
	/**
	 * \brief Getter of `clearColor`.
	 * 
	 * Return the value of the `clearColor` field, corresponding to the color
	 * used to clear the texture when beginning a new render.
	 * It acts as the background color of the scene.
	 * 
	 * \return Value of the `clearColor` field.
	 */
	const Eigen::Vector4f& GetClearColor();
	/**
	 * \brief Getter of `renderTextureID`.
	 * 
	 * Return the value of the `renderTextureID` field, corresponding to the
	 * texture ID created by _OpenGL_ on the graphics card where the scene will
	 * be rendered.
	 * 
	 * \return Value of the `renderTextureID` field.
	 */
	const GLuint& GetRenderTexture() const;
	/**
	 * \brief Getter of `scene`.
	 * 
	 * Return the value of the `scene` field, a pointer corresponding to the
	 * scene that is rendered by the renderer.
	 * 
	 * \return Value of the `scene` field.
	 */
	Scene* GetScene();
	/**
	 * \brief Getter of `nbShaders`.
	 * 
	 * Return the value of the `nbShaders` field, corresponding to the number of
	 * pairs of shaders used by the renderer.
	 * 
	 * \return Value of the `nbShaders` field.
	 */
	unsigned char GetNbShaders();
	/**
	 * \brief Getter of `shaders`.
	 * 
	 * Return the value of the `shaders` field, corresponding to an array of
	 * pointer of pairs of shaders (directly to the `ShadersReader` objects)
	 * used by the renderer.
	 * The size of the array can be accessed with `GetNbShaders()`.
	 * 
	 * \return Value of the `shaders` field.
	 */
	ShadersReader** GetShaders();
	/**
	 * \brief Getter of `renderingPerMaterial`.
	 * 
	 * Return the value of the `renderingPerMaterial` field, corresponding to
	 * the approach of the renderer: per-material (`true`) or one-pass
	 * (`false`).
	 * 
	 * \return Value of the `renderingPerMaterial` field.
	 */
	bool IsRenderingPerMaterial();

	/**
	 * \brief Setter of `clearColor`.
	 * 
	 * Set the value of the `clearColor` field, corresponding to the color used
	 * to clear the texture when beginning a new render.
	 * It acts as the background color of the scene.
	 * 
	 * \param color New clear color.
	 */
	void SetClearColor(Eigen::Vector4f color);
	/**
	 * \brief Setter of `renderingPerMaterial`.
	 * 
	 * Set the value of the `renderingPerMaterial` field, corresponding to the
	 * approach of the renderer: per-material (`true`) or one-pass (`false`).
	 * If the new value is different from the previous one, the renderer will be
	 * automatically re-initialized.
	 * 
	 * \param value New approach to adopt for the renderer: per-material
	 *      (`true`) or one-pass (`false`)
	 */
	void SetRenderingPerMaterial(bool value);
	/**
	 * \brief Setter of `scene`.
	 * 
	 * Set the `scene` field to a new one, then initialize the renderer to
	 * handle it.
	 * The old one will not be destructed.
	 * 
	 * \param scene New scene.
	 */
	void SetScene(Scene* scene);

protected:
	/**
	 * \brief Activate the _OpenGL_ context.
	 * 
	 * Activate the _OpenGL_ context to begin rendering.
	 */
	void ActivateContext();
	/**
	 * \brief Deactivate the _OpenGL_ context.
	 * 
	 * Deactivate the _OpenGL_ context after rendering.
	 * Return to default context.
	 */
	const void DeactivateContext();

	/**
	 * \brief Initialize the scene.
	 * 
	 * If a scene is provided, set the renderer as the renderer of the scene
	 * and initialize shaders and list of faces to draw.
	 */
	void InitScene();
	/**
	 * \brief Initialize the shaders for the one-pass approach.
	 * 
	 * Create the shaders that will handle the one-pass approach.
	 * Virtual function.
	 */
	virtual void InitFullPassShaders();
	/**
	 * \brief Initialize the shaders for the per-material approach.
	 * 
	 * Create as many pairs of shaders as materials in the mesh that handle the
	 * per-material approach.
	 * Virtual function.
	 */
	virtual void InitPerMaterialShaders();

	/**
	 * \brief Delete the array `shaders`.
	 * 
	 * Delete the array `shaders` containing the pointers to pairs of shaders
	 * (directly to the `ShadersReader` objects) and reset the array pointer to
	 * `nullptr`.
	 * The corresponding objects will be deleted.
	 */
	void CleanShaders();

	/**
	 * \brief Application context using this module
	 * 
	 * Pointer to the application context using this module.
	 * Needed to communicate with the rest of the application.
	 */
	void* context = nullptr;

	/**
	 * \brief Clear color of the texture.
	 * 
	 * Color used to clear the texture when beginning a new render.
	 * It acts as the background color of the scene.
	 */
	Eigen::Vector4f clearColor = Eigen::Vector4f(0., 0., 0., 1.);
	/**
	 * \brief Scene to render.
	 * 
	 * Scene that is rendered by the renderer.
	 */
	Scene* scene = nullptr;

	/**
	 * \brief Approch to use for rendering.
	 * 
	 * The approach of the renderer: per-material (`true`) or one-pass
	 * (`false`).
	 */
	bool renderingPerMaterial = false;

	/**
	 * \brief Number of pairs of shaders.
	 * 
	 * Size of the `shaders` array.
	 */
	unsigned char nbShaders = 0;
	/**
	 * \brief Array of pairs of shaders.
	 * 
	 * Array of pointers of pairs of shaders (directly to `ShadersReader`
	 * objects).
	 * Its size must not be less than `nbShaders` value.
	 */
	ShadersReader** shaders = nullptr;

private:
	/**
	 * \brief Initialize common elements of renderers.
	 * 
	 * Create the render FBO and the texture where the scene will be rendered.
	 * Will create objects and buffers on the graphics card.
	 * 
	 * In case of problem, a message is printed in the console.
	 */
	void Init();
	/**
	 * \brief Delete elements created during initialization.
	 * 
	 * Delete objects created on the graphics card.
	 */
	void Clean();

	/**
	 * \brief ID of the render FBO.
	 * 
	 * ID returned by _OpenGL_ when generating the Frame Buffer Object (FBO)
	 * that will be used for rendering, that will contains the texture.
	 */
	GLuint renderFboID;
	/**
	 * \brief ID of the render RBO.
	 * 
	 * ID returned by _OpenGL_ when generating the Render Buffer Object (RBO)
	 * that will be used for rendering, that will contains the depth buffer.
	 */
	GLuint renderRboID;
	/**
	 * \brief ID of the render texture.
	 * 
	 * ID returned by _OpenGL_ when generating the texture that will be used to
	 * render the scene.
	 */
	GLuint renderTextureID;
};

#endif // RENDERERS_RENDERER_H
