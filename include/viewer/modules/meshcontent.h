#ifndef MODULES_MESHCONTENT_H
#define MODULES_MESHCONTENT_H

#include "mesh.h"
#include "modules/module.h"

class MeshContentModule: public GUIModule
{
public:
	MeshContentModule(void* context, std::string filename, Mesh* mesh);
	MeshContentModule(MeshContentModule* module);
	~MeshContentModule();

	void Init();
	void Render();

	Mesh* GetMesh();
	const std::string& GetFilename();
private:
	Mesh* mesh = nullptr;
	std::string filename;

	ImGuiTableFlags tableFlags;
};

#endif // MODULES_MESHCONTENT_H
