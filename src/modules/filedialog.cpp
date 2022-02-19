#include "modules/filedialog.h"

#include <imgui.h>

FileDialogModule::FileDialogModule(Context* context, std::string title,
		std::string formats, bool isSaving)
		: GUIModule(context)
		, formats(formats) {
	this->title = title;

	if (isSaving)
		this->mode = imgui_addons::ImGuiFileBrowser::DialogMode::SAVE;
	else
		this->mode = imgui_addons::ImGuiFileBrowser::DialogMode::OPEN;

	this->Init();
}

FileDialogModule::FileDialogModule(FileDialogModule* module)
		: GUIModule(module->GetContext())
		, formats(module->GetFormats()) {
	this->title = module->GetTitle();

	this->Init();
}

FileDialogModule::~FileDialogModule() {
	delete this->dialog;
}

void FileDialogModule::Init() {
	this->dialog = new imgui_addons::ImGuiFileBrowser();
}

void FileDialogModule::Render() {
	std::string fullName = this->title + "###" + std::to_string(this->id);
	ImGui::OpenPopup(fullName.c_str());
	if (this->dialog->showFileDialog(fullName,
			this->mode, ImVec2(700, 400), this->formats)) {
		this->SendResults();
		this->Kill();
	}
	// TODO: Find how to kill the module when ‘Cancel’ is pressed
}

void FileDialogModule::SendResults() {
	if (this->mode == imgui_addons::ImGuiFileBrowser::DialogMode::OPEN)
		((Context*) this->context)->LoadPLYFile(this->dialog->selected_path);
}

std::string FileDialogModule::GetFormats() {
	return this->formats;
}

imgui_addons::ImGuiFileBrowser::DialogMode FileDialogModule::GetMode() {
	return this->mode;
}
