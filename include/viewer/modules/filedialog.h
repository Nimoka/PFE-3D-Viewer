#ifndef MODULES_FILEDIALOG_H
#define MODULES_FILEDIALOG_H

#include <ImGuiFileBrowser.h>

#include "context.h"
#include "modules/module.h"

/**
 * \brief File dialog module for _Dear ImGui_.
 */
class FileDialogModule: public GUIModule
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `FileDialogModule` constructor.
	 * 
	 * \param context Application context using this module. It will send
	 *      module results and ask to kill it when it is done.
	 * \param title Title of the module.
	 * \param formats Formats allowed to select in the dialog.
	 * \param isSaving Mode of the dialog: either SAVE (`true`) or OPEN
	 *      (`false`).
	 */
	FileDialogModule(Context* context, std::string title,
			std::string formats, bool isSaving);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `FileDialogModule` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param module Module to duplicate.
	 */
	FileDialogModule(FileDialogModule* module);
	/**
	 * \brief Destructor.
	 * 
	 * `FileDialogModule` destructor, will deleting all objects that it created.
	 */
	~FileDialogModule();

	/**
	 * \brief Render the module.
	 * 
	 * Call _Dear ImGui_ instructions to render the module.
	 */
	void Render();

	/**
	 * \brief Getter of `formats`.
	 * 
	 * Return the value of the `formats` field, corresponding to the list of
	 * formats accepted by the dialog.
	 * 
	 * \return Value of the `formats` field.
	 */
	std::string GetFormats();
	/**
	 * \brief Getter of `mode`.
	 * 
	 * Return the value of the `mode` field, corresponding to the mode set for
	 * the dialog.
	 * 
	 * \return Value of the `mode` field.
	 */
	imgui_addons::ImGuiFileBrowser::DialogMode GetMode();

private:
	/**
	 * \brief Initialize the module.
	 * 
	 * Initialize the module: create objects needed.
	 * Will be automatically called during object construction.
	 */
	void Init();

	/**
	 * \brief Send results.
	 * 
	 * Send the results of the dialog by directly calling methods from the
	 * context provided.
	 */
	void SendResults();

	/**
	 * \brief Formats accepted by the dialog.
	 * 
	 * List of formats accepted by the dialog, separated by commas.
	 * Accepts regular expressions.
	 */
	std::string formats;
	/**
	 * \brief Mode of the dialog.
	 * 
	 * Mode of the dialog, set during construction using the `isSaving`
	 * parameter.
	 * Raw value that will be used by the dialog.
	 */
	imgui_addons::ImGuiFileBrowser::DialogMode mode;

	/**
	 * \brief File dialog object.
	 * 
	 * File dialog opened during construction.
	 */
	imgui_addons::ImGuiFileBrowser* dialog = nullptr;
};

#endif // MODULES_FILEDIALOG_H
