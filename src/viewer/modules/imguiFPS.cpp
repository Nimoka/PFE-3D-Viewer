#include "modules/imguiFPS.h"
#include "context.h"

ImGuiFPSModule::ImGuiFPSModule(void* context)
		: GUIModule(context) {
			this->context = context;
		}

ImGuiFPSModule::ImGuiFPSModule(ImGuiFPSModule* module)
		: GUIModule(module->GetContext()) {}

ImGuiFPSModule::~ImGuiFPSModule() {}

void ImGuiFPSModule::ShowFPSWindow(){
	Context* context =  static_cast<Context*> (this->context);
	ImGui::Begin("FPS");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void ImGuiFPSModule::Render() {
	ShowFPSWindow();
}
