#include "modules/module.h"

#include "context.h"

GUIModule::GUIModule(void* context)
		: id(Global::nextModuleID++)
		, context(context) {}

GUIModule::~GUIModule() {}

void GUIModule::Kill() {
	this->readyToDie = true;
	((Context*) this->context)->AskForUpdate();
}

bool GUIModule::IsReadyToDie() {
	return this->readyToDie;
}

unsigned int GUIModule::GetID() {
	return this->id;
}

const std::string& GUIModule::GetTitle() {
	return this->title;
}

void* GUIModule::GetContext() {
	return this->context;
}

void GUIModule::SetTitle(const std::string& value) {
	this->title = value;
}
