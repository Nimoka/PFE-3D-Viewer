#ifndef RENDERER_FORWARD_H
#define RENDERER_FORWARD_H

#include "renderers/renderer.h"

/**
 * \brief Forward shading-based renderer.
 * 
 * Renderer that use forward shading to render the scene, and export the result
 * as a texture usable in `ImGui::Image()`.
 */
class ForwardRenderer: public Renderer
{
public:
	/**
	 * \brief Constructor with no scene.
	 * 
	 * `ForwardRenderer` constructor with no scene provided.
	 * The renderer can’t render while it does not have a scene.
	 * 
	 * \param context Application context using this module.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach
	 *      (`false`, default)
	 */
	ForwardRenderer(void* context, bool renderingPerMaterial = false);
	/**
	 * \brief Constructor with scene.
	 * 
	 * `ForwardRenderer` constructor with a `Scene` object provided.
	 * 
	 * \param context Application context using this module.
	 * \param scene Scene to render.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach
	 *      (`false`, default)
	 */
	ForwardRenderer(void* context, Scene* scene,
			bool renderingPerMaterial = false);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `ForwardRenderer` constructor using an existing object that would be
	 * duplicated. It accepts all types of renderers, that way it is possible to
	 * switch from to another just by usign this constructor.
	 * Elements specific to this type of renderer will be initialized, even if
	 * `renderer` is already a `ForwardRenderer`.
	 * 
	 * \param renderer Renderer to duplicate.
	 */
	ForwardRenderer(Renderer* renderer);
	/**
	 * \brief Destructor.
	 * 
	 * `ForwardRenderer` destructor.
	 * It will deallocate objects also on the graphics card.
	 */
	~ForwardRenderer();

	/**
	 * \brief Render the scene.
	 * 
	 * Render `scene`: activate the _OpenGL_ context, prepare the texture and
	 * the buffers, render the mesh for each pair of shaders, then return to
	 * the default _OpenGL_ context.
	 * Because the size of the window can change at any moment, the size of the
	 * texture generated is always updated using the `size` parameter.
	 * 
	 * \param size Size of the texture to render.
	 */
	void Render(ImVec2 size);

	/**
	 * \brief Update the list of directional lights.
	 * 
	 * Update the list of directional lights as it will be passed to the
	 * shaders (uniform array). Because the number of lights is directly set in
	 * the code of the shaders, shaders should be reloaded.
	 * 
	 * \param reload Reload shaders (`true`, default) or not (`false`). Might
	 *      be useful when adding or removing multiple lights via multiple
	 *      calls of the adding or removing function.
	 */
	void UpdateDirectionalLightList(bool reload = true);
	/**
	 * \brief Update the list of point lights.
	 * 
	 * Update the list of point lights as it will be passed to the shaders
	 * (uniform array). Because the number of lights is directly set in the
	 * code of the shaders, shaders should be reloaded.
	 * 
	 * \param reload Reload shaders (`true`, default) or not (`false`). Might
	 *      be useful when adding or removing multiple lights via multiple
	 *      calls of the adding or removing function.
	 */
	void UpdatePointLightList(bool reload = true);

	/**
	 * \brief Setter of `scene`.
	 * 
	 * Set the `scene` field to a new one, initialize the renderer to handle it,
	 * then update the lists of lights.
	 * The old one will not be destructed.
	 * 
	 * \param scene New scene.
	 */
	void SetScene(Scene* scene);

protected:
	/**
	 * \brief Initialize the shaders for the one-pass approach.
	 * 
	 * Create the shaders that will handle the one-pass approach.
	 */
	void InitFullPassShaders();
	/**
	 * \brief Initialize the shaders for the per-material approach.
	 * 
	 * Create as many pairs of shaders as materials in the mesh that handle the
	 * per-material approach.
	 */
	void InitPerMaterialShaders();

private:
	/**
	 * \brief Number of directional lights in the list.
	 * 
	 * Number of directional lights in the list (splitted into two arrays).
	 * Should be the size of both `directionalLightsDirection` (3 elements per
	 * light) and `directionalLightsIntensity` (3 elements per light).
	 */
	unsigned int nbDirectionalLights = 0;
	/**
	 * \brief Array of directions of the directional lights.
	 * 
	 * Array of directions of the directional lights. There are 3 values per
	 * light (`x`, `y` and `z`), so it should 3 times `nbDirectionalLights`
	 * value.
	 */
	float* directionalLightsDirection = nullptr;
	/**
	 * \brief Array of intensities of the directional lights.
	 * 
	 * Array of intensities of the directional lights. There are 3 values per
	 * light (`r`, `g` and `b`), so it should 3 times `nbDirectionalLights`
	 * value.
	 */
	float* directionalLightsIntensity = nullptr;

	/**
	 * \brief Number of point lights in the list.
	 * 
	 * Number of point lights in the list (splitted into two arrays).
	 * Should be the size of both `pointLightsPosition` (3 elements per light)
	 * and `pointLightsIntensity` (3 elements per light).
	 */
	unsigned int nbPointLights = 0;
	/**
	 * \brief Array of positions of the point lights.
	 * 
	 * Array of positions of the point lights. There are 3 values per
	 * light (`x`, `y` and `z`), so it should 3 times `nbPointLights` value.
	 */
	float* pointLightsPosition = nullptr;
	/**
	 * \brief Array of intensities of the point lights.
	 * 
	 * Array of intensities of the point lights. There are 3 values per
	 * light (`r`, `g` and `b`), so it should 3 times `nbPointLights` value.
	 */
	float* pointLightsIntensity = nullptr;
};

#endif // RENDERER_FORWARD_H
