#ifndef GUIMODULES_PLYCONTENT_H
#define GUIMODULES_PLYCONTENT_H

#include "guimodules/module.h"
#include "mesh.h"

class PLYContentModule: public GUIModule
{
public:
	PLYContentModule(std::string name, Mesh* mesh);
	PLYContentModule(PLYContentModule* module);
	~PLYContentModule();

	void Init();
	void Render();

	Mesh* GetMesh();
private:
	Mesh* mesh;

	ImGuiTableFlags tableFlags;
};

#endif // GUIMODULES_PLYCONTENT_H
