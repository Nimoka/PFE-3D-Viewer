#ifndef GUIMODULES_PLYCONTENT_H
#define GUIMODULES_PLYCONTENT_H

#include "guimodules/module.h"
#include "mesh.h"

class PlyContentModule: public GUIModule
{
public:
	PlyContentModule(std::string name, Mesh* mesh);
	PlyContentModule(PlyContentModule* module);
	~PlyContentModule();

	void Init();
	void Render();

	Mesh* GetMesh();
private:
	Mesh* mesh;

	ImGuiTableFlags tableFlags;
};

#endif // GUIMODULES_PLYCONTENT_H
