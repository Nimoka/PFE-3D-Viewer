#include "modules/imguidemo.h"

ImGuiDemoModule::ImGuiDemoModule(void* context)
: GUIModule(context) {}

ImGuiDemoModule::ImGuiDemoModule(ImGuiDemoModule* module)
: GUIModule(module->GetContext()) {}

ImGuiDemoModule::~ImGuiDemoModule() {}

void ImGuiDemoModule::Render() {
	ImGui::ShowDemoWindow();
}
