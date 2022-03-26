#ifndef MODULES_MODULE_H
#define MODULES_MODULE_H

#include <iostream>

#include <imgui.h>

#include "utils.h"

/**
 * \brief Virtual module for _Dear ImGui_.
 * 
 * Stores useful data for a good _ImGui_ rendering of subwindows, like a custom
 * ID to avoid problems when multiple subwindows have the same title.
 * Defines methods used for each module, like `Kill()`.
 */
class GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `GUIModule` constructor.
	 * 
	 * \param context Application context using this module. It will send
	 *      module results and ask to kill it when it is done.
	 */
	GUIModule(void* context);
	/**
	 * \brief Virtual destructor.
	 * 
	 * `GUIModule` destructor, that should be called whenever a derivated class
	 * is destructed.
	 */
	virtual ~GUIModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 * Pure virtual: there is nothing to render here, but every derivated
	 * objected need to.
	 */
	virtual void Render() = 0;

	/**
	 * \brief Kill the module.
	 * 
	 * Prepare the module to be killed.
	 * 
	 * This will not destruct the module, to avoid `SEGFAULT` from the
	 * application context. It set `readyToDie` as `true` and ask the context to
	 * update its list of modules and destruct every module that are ready to
	 * die as an offering to the memory.
	 */
	void Kill();

	/**
	 * \brief Getter of `readyToDie`.
	 * 
	 * Return the value of the `readyToDie` field.
	 * 
	 * \return Module is ready to die (`true`) or not (`false`)
	 */
	bool IsReadyToDie();

	/**
	 * \brief Getter of `id`.
	 * 
	 * Return the value of the `id` field, corresponding to the ID of the
	 * subwindow created by the module.
	 * 
	 * \return Value of the `id` field.
	 */
	unsigned int GetID();
	/**
	 * \brief Getter of `title`.
	 * 
	 * Return the value of the `title` field, corresponding to the title of the
	 * subwindow created by the module.
	 * 
	 * \return Value of the `title` field.
	 */
	const std::string& GetTitle();
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
	 * \brief Setter of `title`.
	 * 
	 * Set a new value for the `title` field, corresponding to the title of the
	 * subwindow created by the module.
	 * 
	 * \param value New value for the `title` field.
	 */
	void SetTitle(const std::string& value);

protected:
	/**
	 * \brief ID of the subwindow.
	 * 
	 * ID of the subwindow created by this module.
	 * Used to handle multiple subwindows with the same title using _Dear
	 * ImGui_. Needed to avoid to have multiple subwindows’ contents in the same
	 * subwindow.
	 */
	unsigned int id;
	/**
	 * \brief Title of the subwindow.
	 * 
	 * Title of the subwindow created by this module.
	 */
	std::string title;
	/**
	 * \brief Application context using this module
	 * 
	 * Pointer to the application context using this module.
	 * Needed to communicate with the rest of the application.
	 */
	void* context = nullptr;

	/**
	 * \brief Is the module ready to die?
	 * 
	 * Tells if the module is ready to die or not.
	 * Will be checked when the application context will remove no-more-used
	 * modules from the list of modules to render and interact with.
	 */
	bool readyToDie = false;
};

#endif // MODULES_MODULE_H
