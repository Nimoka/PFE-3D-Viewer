#include "modules/shaderscontent.h"

ShadersContentModule::ShadersContentModule(void* context, ShaderReader* shader)
		: GUIModule(context)
		, shader(shader) {
	this->title = "Shaders content";

	this->Init();
}

ShadersContentModule::ShadersContentModule(ShadersContentModule* module)
		: GUIModule(module->GetContext())
		, shader(module->GetShader()) {
	this->title = module->GetTitle();

	this->Init();
}

ShadersContentModule::~ShadersContentModule() {}

void ShadersContentModule::Init() {}

void ShadersContentModule::Render() {
	if (this->shader != nullptr) {
		if (ImGui::Begin(std::string(this->title + "###"
				+ std::to_string(this->id)).c_str())) {
			ImGui::Text("Informations:");
			ImGui::Text("  Vertex shader: %s",
					this->shader->GetVertexShaderPath().c_str());
			ImGui::Text("  Fragment shader: %s",
					this->shader->GetFragmentShaderPath().c_str());
			ImGui::Text("  Dynamic: %s",
					(this->shader->AreDynamic() ? "yes" : "no"));
			ImGui::Separator();
			if (this->shader->IsLoaded()) {
				if (ImGui::CollapsingHeader("Vertex shader")) {
					ImGui::TextUnformatted(
							this->shader->GetVertexShaderContent().c_str());
				}
				if (ImGui::CollapsingHeader("Fragment shader")) {
					ImGui::TextUnformatted(
							this->shader->GetFragmentShaderContent().c_str());
				}
			} else {
				ImGui::Text("Not loaded.");
			}
		}
		ImGui::End();
	}
}

ShaderReader* ShadersContentModule::GetShader() {
	return this->shader;
}
