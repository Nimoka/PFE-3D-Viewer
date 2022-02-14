#ifndef MODULES_IMGUIDEMO_H
#define MODULES_IMGUIDEMO_H

#include "modules/module.h"

class ImGuiDemoModule: public GUIModule
{
public:
	ImGuiDemoModule(void* context);
	ImGuiDemoModule(ImGuiDemoModule* module);
	~ImGuiDemoModule();

	void Render();
};

#endif // MODULES_IMGUIDEMO_H
