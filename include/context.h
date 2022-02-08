#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include "modules/module.h"
#include "plyreader.h"

class Context
{
public:
	Context();
	~Context();

	void LoadPLYFile(std::string filepath);

	void Render();

	int GetNewModuleID();

private:
	void RenderMenuBar();

	std::vector<GUIModule*> modules;
	std::vector<PLYReader*> readers;

	int nextModuleID;
};

#endif // CONTEXT_H
