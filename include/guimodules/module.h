#ifndef GUIMODULES_MODULE_H
#define GUIMODULES_MODULE_H

#include <iostream>

#include <imgui.h>

#include "utils.h"

class GUIModule
{
public:
	GUIModule() { this->id = Global::moduleIdCounter++; }
	~GUIModule() {}
	virtual void Render() = 0;

	int GetID() { return this->id; }
	std::string GetTitle() { return this->title; }

	void SetTitle(std::string value) { this->title = value; }
protected:
	int id;
	std::string title;
};

#endif // GUIMODULES_MODULE_H
