#ifndef MODULES_VIEWER_H
#define MODULES_VIEWER_H

#include <string>

#include <imgui.h>

#include "mesh.h"
#include "modules/module.h"
#include "renderers/renderer.h"

/**
 * \brief Viewer module for _Dear ImGui_.
 * 
 * Module that call the renderer and display the result in the available space
 * of the main window.
 */
class ViewerModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `ViewerModule` constructor.
	 * 
	 *  \param context Application context using this module.
	 */
	ViewerModule(void* context);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `ViewerModule` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param module Module to duplicate.
	 */
	ViewerModule(ViewerModule* module);
	/**
	 * \brief Destructor.
	 * 
	 * `ViewerModule` destructor.
	 */
	~ViewerModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

	/**
	 * \brief Getter of `mesh`.
	 * 
	 * Return the pointer to the mesh rendered by `renderer` if any.
	 * Return `nullptr` if either the `renderer` field, if the `scene` field of
	 * `renderer` or the `mesh` field of the `scene` of the `renderer` is
	 * `nullptr`.
	 * 
	 * \return Value returned by the getter of `mesh` of the renderer.
	 */
	Mesh* GetMesh();
	/**
	 * \brief Getter of `renderer`.
	 * 
	 * Return the value of the `renderer` field, a pointer corresponding to the
	 * renderer whose result is displayed.
	 * 
	 * \return Value of the `renderer` field.
	 */
	Renderer* GetRenderer();

	/**
	 * \brief Setter of `renderer`.
	 * 
	 * Set a new renderer.
	 * The old one will be destructed.
	 * 
	 * \param renderer New renderer.
	 */
	void SetRenderer(Renderer* renderer);


private:
	/**
	 * \brief Initialize the module.
	 * 
	 * Initialize the module: set _ImGui_ flags for subwindow style.
	 * Will be automatically called during object construction.
	 */
	void Init();

	/**
	 * \brief Renderer.
	 * 
	 * Renderer that contains all elements to do a render.
	 * Its result will be displayed as a image in an `ImGui::Image`
	 * instruction.
	 */
	Renderer* renderer = nullptr;

	/**
	 * \brief Flags describing style of the _ImGui_ subwindow.
	 * 
	 * Flags describing style of the _ImGui_ subwindow.
	 * Initialized during `Init()`.
	 */
	ImGuiWindowFlags flags;
};

#endif // MODULES_VIEWER_H
