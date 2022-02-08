#include "modules/filedialog.h"

#include <imgui.h>

FileDialogModule::FileDialogModule(Context* context, std::string title,
		std::string formats, bool isSaving)
: GUIModule(context) {
	this->title = title;
	this->formats = formats;
	if (isSaving)
		this->mode = imgui_addons::ImGuiFileBrowser::DialogMode::SAVE;
	else
		this->mode = imgui_addons::ImGuiFileBrowser::DialogMode::OPEN;

	this->Init();
}

FileDialogModule::FileDialogModule(FileDialogModule* module)
: GUIModule(module->GetContext()) {
	this->title = module->GetTitle();
	this->formats = module->GetFormats();

	this->Init();
}

FileDialogModule::~FileDialogModule() {
	delete this->dialog;
}

void FileDialogModule::Init() {
	this->dialog = new imgui_addons::ImGuiFileBrowser();
}

void FileDialogModule::Render() {
	ImGui::OpenPopup(this->title.c_str());
	if (this->dialog->showFileDialog(this->title, this->mode, ImVec2(700, 310), this->formats)) {
		this->SendResults();
		this->readyToDie = true;
	}
}

void FileDialogModule::SendResults() {
	std::cout << this->dialog->selected_path << std::endl;
	if (this->mode == imgui_addons::ImGuiFileBrowser::DialogMode::OPEN)
		((Context*) this->context)->LoadPLYFile(this->dialog->selected_path);
}

std::string FileDialogModule::GetFormats() {
	return this->formats;
}

imgui_addons::ImGuiFileBrowser::DialogMode FileDialogModule::GetMode() {
	return this->mode;
}
