#include "context.h"

#include "modules/plycontent.h"

Context::Context() {
}

Context::~Context() {
	for (auto i: this->modules)
		delete i;
	for (auto i: this->readers)
		delete i;
}

void Context::LoadPLYFile(std::string filepath) {
	PLYReader* reader = new PLYReader(filepath);
	if (reader->Load()) {
		this->modules.push_back(new PLYContentModule(filepath, reader->GetMesh()));
		this->readers.push_back(reader);
		return;
	}
	delete reader;
	// TODO: Throw exception
}

void Context::Render() {
	this->RenderMenuBar();
	for (auto i: this->modules)
		i->Render();
}

int Context::GetNewModuleID() {
	return this->nextModuleID++;
}

void Context::RenderMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open document…", "Ctrl+O")) {}
			if (ImGui::MenuItem("Close document", "Ctrl+W")) {}
			if (ImGui::MenuItem("Quit", "Ctrl+Q")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
