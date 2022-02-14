#include "modules/module.h"

#include "context.h"

GUIModule::GUIModule(void* context) {
	this->id = Global::nextModuleID++;
	this->context = context;
	this->readyToDie = false;
}

void GUIModule::Kill() {
	this->readyToDie = true;
	((Context*) this->context)->AskForUpdate();
}

bool GUIModule::IsReadyToDie() {
	return this->readyToDie;
}

int GUIModule::GetID() {
	return this->id;
}

std::string GUIModule::GetTitle() {
	return this->title;
}

void* GUIModule::GetContext() {
	return this->context;
}

void GUIModule::SetTitle(std::string value) {
	this->title = value;
}
