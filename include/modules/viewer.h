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

	void Init();
	void Render();

	void SetMesh(Mesh* mesh);
	void SetRenderer(Renderer* renderer);

	Mesh* GetMesh();
	Renderer* GetRenderer();

private:
	Renderer* renderer;

	ImGuiWindowFlags flags;
};

#endif // MODULES_VIEWER_H
