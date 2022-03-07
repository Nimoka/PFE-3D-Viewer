#include "modules/shaderscontent.h"

ShadersContentModule::ShadersContentModule(void* context, ShadersReader* shaders)
		: GUIModule(context)
		, shaders(shaders) {
	this->title = "Shaders content";

	this->Init();
}

ShadersContentModule::ShadersContentModule(ShadersContentModule* module)
		: GUIModule(module->GetContext())
		, shaders(module->GetShaders()) {
	this->title = module->GetTitle();

	this->Init();
}

ShadersContentModule::~ShadersContentModule() {}

void ShadersContentModule::Init() {}

void ShadersContentModule::Render() {
	if (this->shaders != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###"
				+ std::to_string(this->id)).c_str())) {
			ImGui::Text("Informations:");
			ImGui::Text("  Vertex shader: %s",
					this->shaders->GetVertexShaderPath().c_str());
			ImGui::Text("  Fragment shader: %s",
					this->shaders->GetFragmentShaderPath().c_str());
			ImGui::Text("  Are dynamic: %s",
					(this->shaders->AreDynamic() ? "yes" : "no"));
			ImGui::Separator();
			if (this->shaders->AreLoaded()) {
				if (ImGui::CollapsingHeader("Vertex shader source code")) {
					ImGui::TextUnformatted(
							this->shaders->GetVertexShaderSource().c_str());
				}
				if (ImGui::CollapsingHeader("Fragment shader source code")) {
					ImGui::TextUnformatted(
							this->shaders->GetFragmentShaderSource().c_str());
				}
			} else {
				ImGui::Text("Not loaded.");
			}
		}
		ImGui::End();
	}
}

ShadersReader* ShadersContentModule::GetShaders() {
	return this->shaders;
}
