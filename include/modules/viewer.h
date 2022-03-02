#ifndef MODULES_VIEWER_H
#define MODULES_VIEWER_H

#include <imgui.h>
#include <string>

#include "mesh.h"
#include "modules/module.h"
#include "renderers/renderer.h"

class ViewerModule: public GUIModule
{
public:
	ViewerModule(void* context);
	ViewerModule(ViewerModule* module);
	~ViewerModule();

	void Render();

	void SetRenderer(Renderer* renderer);

	Mesh* GetMesh();
	Renderer* GetRenderer();

private:
	void Init();

	Renderer* renderer = nullptr;

	ImGuiWindowFlags flags;
};

#endif // MODULES_VIEWER_H
