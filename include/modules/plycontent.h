#ifndef MODULES_PLYCONTENT_H
#define MODULES_PLYCONTENT_H

#include "context.h"
#include "mesh.h"
#include "modules/module.h"

class PLYContentModule: public GUIModule
{
public:
	PLYContentModule(Context* context, std::string name, Mesh* mesh);
	PLYContentModule(PLYContentModule* module);
	~PLYContentModule();

	void Init();
	void Render();

	Mesh* GetMesh();
private:
	Mesh* mesh;

	ImGuiTableFlags tableFlags;
};

#endif // MODULES_PLYCONTENT_H
