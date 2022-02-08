#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include <ImGuiFileBrowser.h>

#include "modules/module.h"
#include "plyreader.h"

class Context
{
public:
	Context();
	~Context();

	void CreateOpenPLYFileSelectionDialog();
	void CreateSavePLYFileSelectionDialog();
	void LoadPLYFile(std::string filepath);

	void Render();
	void Update();

	bool IsReadyToDie();

	int GetNewModuleID();

private:
	void RenderMenuBar();

	std::vector<GUIModule*> modules;
	std::vector<PLYReader*> readers;

	imgui_addons::ImGuiFileBrowser* fileDialog;

	int nextModuleID;

	bool readyToDie;
};

#endif // CONTEXT_H
