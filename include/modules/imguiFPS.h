#ifndef MODULES_IMGUIFPS_H
#define MODULES_IMGUIFPS_H

#include "modules/module.h"

class ImGuiFPSModule: public GUIModule
{
public:
	ImGuiFPSModule(void* context);
	ImGuiFPSModule(ImGuiFPSModule* module);
    void ShowFPSWindow();
	~ImGuiFPSModule();

	void Render();
};

#endif // MODULES_IMGUIFPS_H
