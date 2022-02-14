#include "modules/viewer.h"

ViewerModule::ViewerModule(void* context)
: GUIModule(context) {
	this->title = "Viewer";

	this->Init();
}

ViewerModule::ViewerModule(ViewerModule* module)
: GUIModule(module->GetContext()) {
	this->title = module->GetTitle();
	this->mesh = module->GetMesh();

	this->Init();
}

ViewerModule::~ViewerModule() {}

void ViewerModule::Init() {
	this->flags = ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoBackground;
}

void ViewerModule::Render() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);

	ImGui::Begin(std::string(this->title + "###" + std::to_string(this->id)).c_str(), nullptr, this->flags);
	ImGui::Text("Placeholder");
	ImGui::End();
}

void ViewerModule::SetMesh(Mesh* mesh) {
	this->mesh = mesh;
}

Mesh* ViewerModule::GetMesh() {
	return this->mesh;
}
