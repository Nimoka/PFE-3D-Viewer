#include "modules/viewer.h"

#include "renderers/forward.h"

ViewerModule::ViewerModule(void* context)
: GUIModule(context) {
	this->title = "Viewer";

	this->Init();
}

ViewerModule::ViewerModule(ViewerModule* module)
: GUIModule(module->GetContext()) {
	this->title = module->GetTitle();
	this->renderer = module->GetRenderer();

	this->Init();
}

ViewerModule::~ViewerModule() {}

void ViewerModule::Init() {
	this->flags = ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus;

	this->renderer = new ForwardRenderer();
}

void ViewerModule::Render() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 size = viewport->WorkSize;
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(size);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin(std::string(this->title + "###" + std::to_string(this->id)).c_str(), nullptr, this->flags);
	if (this->renderer != nullptr) {
		this->renderer->Render();
		ImGui::Image((ImTextureID) this->renderer->GetTextureIDPointer(), size, ImVec2(0, 1), ImVec2(1, 0));
	} else {
		ImGui::Text("No renderer");
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void ViewerModule::SetRenderer(Renderer* renderer) {
	if (this->renderer != nullptr)
		delete this->renderer;
	this->renderer = renderer;
}

void ViewerModule::SetMesh(Mesh* mesh) {
	if (this->renderer != nullptr)
		this->renderer->GetScene()->SetMesh(mesh);
}

Renderer* ViewerModule::GetRenderer() {
	return this->renderer;
}

Mesh* ViewerModule::GetMesh() {
	return this->renderer->GetScene()->GetMesh();
}
