#ifndef MODULES_VIEWER_H
#define MODULES_VIEWER_H

#include <imgui.h>

#include "mesh.h"
#include "modules/module.h"

class ViewerModule: public GUIModule
{
public:
	ViewerModule(void* context);
	ViewerModule(ViewerModule* module);
	~ViewerModule();

	void Init();
	void Render();

	void SetMesh(Mesh* mesh);

	Mesh* GetMesh();

private:
	Mesh* mesh;

	ImGuiWindowFlags flags;
};

#endif // MODULES_VIEWER_H
