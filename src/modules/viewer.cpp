#include "modules/viewer.h"

#include "renderers/simple.h"

ViewerModule::ViewerModule(void* context)
		: GUIModule(context) {
	this->title = "Viewer";
	this->renderer = new SimpleRenderer(context);

	this->Init();
}

ViewerModule::ViewerModule(ViewerModule* module)
		: GUIModule(module->GetContext())
		, renderer(module->GetRenderer()) {
	this->title = module->GetTitle();

	this->Init();
}

ViewerModule::~ViewerModule() {}

void ViewerModule::Render() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 size = viewport->WorkSize;
	ImVec2 pos = viewport->WorkPos;

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin(std::string(this->title + "###"
			+ std::to_string(this->id)).c_str(), nullptr, this->flags);
	if (this->renderer != nullptr) {
		this->renderer->Render(size);
		ImGui::Image(reinterpret_cast<ImTextureID>(
				this->renderer->GetRenderTexture()),
				size, ImVec2(0, 1), ImVec2(1, 0));
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

Renderer* ViewerModule::GetRenderer() {
	return this->renderer;
}

Mesh* ViewerModule::GetMesh() {
	if (this->renderer != nullptr) {
		if (this->renderer->GetScene() != nullptr)
			return this->renderer->GetScene()->GetMesh();
	}
	return nullptr;
}

void ViewerModule::Init() {
	this->flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus;
}
