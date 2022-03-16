#ifndef MODULES_SHADERSCONTENT_H
#define MODULES_SHADERSCONTENT_H

#include "modules/module.h"
#include "shadersreader.h"

class ShadersContentModule: public GUIModule
{
public:
	ShadersContentModule(void* context,
			unsigned char nbShaders, ShadersReader** shader);
	ShadersContentModule(ShadersContentModule* module);
	~ShadersContentModule();

	void Init();
	void Render();

	unsigned char GetNbShaders();
	ShadersReader** GetShaders();

	void SetShaders(unsigned char nbShaders, ShadersReader** shaders);

private:
	void CleanShaders();

	unsigned char nbShaders = 0;
	ShadersReader** shaders = nullptr;
};

#endif // MODULES_SHADERSCONTENT_H
