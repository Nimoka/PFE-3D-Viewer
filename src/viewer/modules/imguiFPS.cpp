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
	ImGui::Text("Application average %.3f (FPS)",
			 1.0/ context->GetDeltaTime());
	ImGui::End();
}

void ImGuiFPSModule::Render() {
	ShowFPSWindow();
}
