#ifndef MODULES_IMGUIDEMO_H
#define MODULES_IMGUIDEMO_H

#include "modules/module.h"

/**
 * \brief _ImGui_ demo module for _Dear ImGui_.
 * 
 * An empty module created to add the default demo of _Dear ImGui_ to the
 * application. It allows to test some features, and really to check for new UI
 * implementation.
 * 
 * **Caution:** This module should be unique at any moment: subwindows handling
 * in _Dear ImGui_ is based on title (or title + ID if any), having multiple
 * modules will probably lead to having multiple subwindows with the name, and
 * so the content of all `ImGuiDemoModule` objects will be displayed in the
 * same subwindow and `Render()` is called.
 */
class ImGuiDemoModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `ImGuiDemoModule` constructor.
	 * 
	 * \param context Application context using this module. It will ask to
	 *      kill it when it is done.
	 */
	ImGuiDemoModule(void* context);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `ImGuiDemoModule` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param module Module to duplicate.
	 */
	ImGuiDemoModule(ImGuiDemoModule* module);
	/**
	 * \brief Destructor.
	 * 
	 * `ImGuiDemoModule` destructor.
	 */
	~ImGuiDemoModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();
};

#endif // MODULES_IMGUIDEMO_H
