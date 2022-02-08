#ifndef MODULES_MODULE_H
#define MODULES_MODULE_H

#include <iostream>

#include <imgui.h>

// #include "context.h"
#include "utils.h"

class GUIModule
{
public:
	GUIModule(void* context) {
		this->id = Global::nextModuleID++;
		this->context = context;
		this->readyToDie = false;
	}
	virtual ~GUIModule() {}
	virtual void Render() = 0;

	bool IsReadyToDie() { return this->readyToDie; }

	int GetID() { return this->id; }
	std::string GetTitle() { return this->title; }
	void* GetContext() { return this->context; }

	void SetTitle(std::string value) { this->title = value; }

protected:
	int id;
	std::string title;
	void* context;

	bool readyToDie;
};

#endif // MODULES_MODULE_H
