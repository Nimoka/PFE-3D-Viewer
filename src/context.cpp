#include "context.h"

#include "modules/filedialog.h"
#include "modules/plycontent.h"

Context::Context() {
	this->readyToDie = false;
	this->fileDialog = new imgui_addons::ImGuiFileBrowser();
}

Context::~Context() {
	for (auto i: this->modules)
		delete i;
	for (auto i: this->readers)
		delete i;
	if (this->fileDialog != NULL)
		delete this->fileDialog;
}

void Context::CreateOpenPLYFileSelectionDialog() {
	this->modules.push_back(new FileDialogModule(this, "Open file", ".ply", false));
}

// void Context::CreateSavePLYFileSelectionDialog() {
// 	this->modules.push_back(new FileDialogModule(this, "Save file", ".ply", false));
// }

void Context::LoadPLYFile(std::string filepath) {
	PLYReader* reader = new PLYReader(filepath);
	if (reader->Load()) {
		std::string filename = filepath.substr(filepath.rfind('/') + 1);
		this->modules.push_back(new PLYContentModule(this, filename, reader->GetMesh()));
		this->readers.push_back(reader);
		return;
	}
	delete reader;
	// TODO: Throw exception
}

void Context::Update() {
	for (auto it = this->modules.begin(); it != this->modules.end();) {
		if ((*it)->IsReadyToDie()) {
			delete *it;
			it = this->modules.erase(it);
		} else {
			it++;
		}
	}
}

void Context::Render() {
	this->RenderMenuBar();
	for (auto i: this->modules)
		i->Render();

	Update();
}

int Context::GetNewModuleID() {
	return this->nextModuleID++;
}

bool Context::IsReadyToDie() {
	return this->readyToDie;
}

void Context::RenderMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				this->CreateOpenPLYFileSelectionDialog();
			// if (ImGui::MenuItem("Save...", "Ctrl+W"))
			// 	this->CreateSavePLYFileSelectionDialog();
			if (ImGui::MenuItem("Quit", "Ctrl+Q"))
				this->readyToDie = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
