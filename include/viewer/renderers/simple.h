#ifndef RENDERERS_SIMPLE_H
#define RENDERERS_SIMPLE_H

#include "renderers/renderer.h"

/**
 * \brief Simple renderer.
 * 
 * Renderer that use only mesh colors to render the scene, and export the result
 * as a texture usable in `ImGui::Image()`.
 */
class SimpleRenderer: public Renderer
{
public:
	/**
	 * \brief Constructor with no scene.
	 * 
	 * `SimpleRenderer` constructor with no scene provided.
	 * The renderer can’t render while it does not have a scene.
	 * 
	 * \param context Application context using this module.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach
	 *      (`false`, default)
	 */
	SimpleRenderer(void* context, bool renderingPerMaterial = false);
	/**
	 * \brief Constructor with scene.
	 * 
	 * `SimpleRenderer` constructor with a `Scene` object provided.
	 * 
	 * \param context Application context using this module.
	 * \param scene Scene to render.
	 * \param renderingPerMaterial Initialize the renderer to render with a
	 *      per-material approach (`true`) or a one-pass approach
	 *      (`false`, default)
	 */
	SimpleRenderer(void* context, Scene* scene,
			bool renderingPerMaterial = false);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `SimpleRenderer` constructor using an existing object that would be
	 * duplicated. It accepts all types of renderers, that way it is possible to
	 * switch from to another just by usign this constructor.
	 * Elements specific to this type of renderer will be initialized, even if
	 * `renderer` is already a `SimpleRenderer`.
	 * 
	 * \param renderer Renderer to duplicate.
	 */
	SimpleRenderer(Renderer* renderer);
	/**
	 * \brief Destructor.
	 * 
	 * `SimpleRenderer` destructor.
	 * It will deallocate objects also on the graphics card.
	 */
	~SimpleRenderer();

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
	 * \brief (Not implemented)
	 * 
	 * It does nothing, it is just here to fulfill the pure virtual function,
	 * that is useful for other types of renderers.
	 * 
	 * \param reload (Unused)
	 */
	void UpdateDirectionalLightList(bool reload = true);
	/**
	 * \brief (Not implemented)
	 * 
	 * It does nothing, it is just here to fulfill the pure virtual function,
	 * that is useful for other types of renderers.
	 * 
	 * \param reload (Unused)
	 */
	void UpdatePointLightList(bool reload = true);

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
};

#endif // RENDERERS_SIMPLE_H
