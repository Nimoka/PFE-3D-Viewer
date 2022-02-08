#ifndef MODULES_FILEDIALOG_H
#define MODULES_FILEDIALOG_H

#include "context.h"
#include "modules/module.h"

#include <ImGuiFileBrowser.h>

class FileDialogModule: public GUIModule
{
public:
	FileDialogModule(Context* context, std::string title,
			std::string formats, bool isSaving);
	FileDialogModule(FileDialogModule* module);
	~FileDialogModule();

	void Init();
	void Render();

	void SendResults();

	std::string GetPath();
	std::string GetFormats();
	imgui_addons::ImGuiFileBrowser::DialogMode GetMode();

private:
	std::string path;
	std::string formats;
	imgui_addons::ImGuiFileBrowser::DialogMode mode;

	imgui_addons::ImGuiFileBrowser* dialog;
};

#endif // MODULES_FILEDIALOG_H
