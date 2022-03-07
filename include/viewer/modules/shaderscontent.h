#ifndef MODULES_SHADERSCONTENT_H
#define MODULES_SHADERSCONTENT_H

#include "modules/module.h"
#include "shaderreader.h"

class ShadersContentModule: public GUIModule
{
public:
	ShadersContentModule(void* context, ShaderReader* shader);
	ShadersContentModule(ShadersContentModule* module);
	~ShadersContentModule();

	void Init();
	void Render();

	ShaderReader* GetShader();
private:
	ShaderReader* shader;
};

#endif // MODULES_SHADERSCONTENT_H
