#include "modules/shaderscontent.h"

ShadersContentModule::ShadersContentModule(void* context,
		unsigned char nbShaders, ShadersReader** shaders)
		: GUIModule(context) {
	this->title = "Shaders content";
	this->SetShaders(nbShaders, shaders);

	this->Init();
}

ShadersContentModule::ShadersContentModule(ShadersContentModule* module)
		: GUIModule(module->GetContext()) {
	this->title = module->GetTitle();
	this->SetShaders(module->GetNbShaders(), module->GetShaders());

	this->Init();
}

ShadersContentModule::~ShadersContentModule() {}

void ShadersContentModule::Init() {}

void ShadersContentModule::Render() {
	if (this->shaders != nullptr) {
		ShadersReader* currentShaders;
		for (unsigned char i = 0; i < this->nbShaders; i++) {
			currentShaders = this->shaders[i];

			if (currentShaders == nullptr)
				continue;

			if (ImGui::Begin(std::string(this->title
					+ " #" + std::to_string((unsigned int) i)
					+ "###" + std::to_string(this->id)).c_str())) {
				ImGui::Text("Informations:");
				ImGui::TextWrapped("  Vertex shader: %s",
						currentShaders->GetVertexShaderPath().c_str());
				ImGui::TextWrapped("  Fragment shader: %s",
						currentShaders->GetFragmentShaderPath().c_str());
				ImGui::Text("  Are dynamic: %s",
						(currentShaders->AreDynamic() ? "yes" : "no"));
				if (this->nbShaders > 1)
					ImGui::Text("  Used for material: %u", ((unsigned int) i));
				ImGui::Separator();
				if (currentShaders->AreLoaded()) {
					if (ImGui::CollapsingHeader("Vertex shader source code")) {
						ImGui::TextUnformatted(
								currentShaders->
										GetVertexShaderSource().c_str());
					}
					if (ImGui::CollapsingHeader(
							"Fragment shader source code")) {
						ImGui::TextUnformatted(
								currentShaders->
										GetFragmentShaderSource().c_str());
					}
				} else {
					ImGui::Text("Not loaded.");
				}
			}
			ImGui::End();
		}
	}
}

unsigned char ShadersContentModule::GetNbShaders() {
	return this->nbShaders;
}

ShadersReader** ShadersContentModule::GetShaders() {
	return this->shaders;
}

void ShadersContentModule::SetShaders(
		unsigned char nbShaders, ShadersReader** shaders) {
	this->CleanShaders();

	this->nbShaders = nbShaders;
	this->shaders = (ShadersReader**) malloc(sizeof(void*) * nbShaders);
	for (unsigned int i = 0; i < nbShaders; i++)
		this->shaders[i] = shaders[i];
}

void ShadersContentModule::CleanShaders() {
	if (this->shaders != nullptr)
		delete this->shaders;
	this->nbShaders = 0;
}
