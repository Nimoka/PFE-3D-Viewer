#ifndef MODULES_SHADERSCONTENT_H
#define MODULES_SHADERSCONTENT_H

#include "modules/module.h"
#include "shadersreader.h"

class ShadersContentModule: public GUIModule
{
public:
	ShadersContentModule(void* context, ShadersReader* shader);
	ShadersContentModule(ShadersContentModule* module);
	~ShadersContentModule();

	void Init();
	void Render();

	ShadersReader* GetShaders();
	void SetShaders(ShadersReader* shaders);
private:
	ShadersReader* shaders;
};

#endif // MODULES_SHADERSCONTENT_H
