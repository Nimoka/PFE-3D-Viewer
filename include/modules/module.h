#ifndef MODULES_MODULE_H
#define MODULES_MODULE_H

#include <iostream>

#include <imgui.h>

// #include "context.h"
#include "utils.h"

class GUIModule
{
public:
	GUIModule(void* context);
	virtual ~GUIModule() {}

	virtual void Render() = 0;

	void Kill();

	bool IsReadyToDie();

	unsigned int GetID();
	std::string GetTitle();
	void* GetContext();

	void SetTitle(std::string value);

protected:
	unsigned int id;
	std::string title;
	void* context;

	bool readyToDie;
};

#endif // MODULES_MODULE_H
