#ifndef MODULES_MESSAGE_H
#define MODULES_MESSAGE_H

#include "modules/module.h"

class MessageModule: public GUIModule
{
public:
	MessageModule(void* context, std::string message);
	~MessageModule();

	void Render();

	std::string GetMessage();

protected:
	std::string message;
	ImGuiWindowFlags flags;

private:
	void Init();
};

class LoadingMessageModule: public MessageModule
{
public:
	LoadingMessageModule(void* context, std::string message);
	~LoadingMessageModule();

	void Render();

private:
	ImU32 accentColor;
	ImU32 backgroundColor;
};

class ProcessingMessageModule: public MessageModule
{
public:
	ProcessingMessageModule(void* context, std::string message,
			int* currentValue, int* expectedValue);
	~ProcessingMessageModule();

	void Render();

private:
	int* currentValue = nullptr;
	int* expectedValue = nullptr;

	ImU32 accentColor;
	ImU32 backgroundColor;
};

class AlertMessageModule: public MessageModule
{
public:
	AlertMessageModule(void* context, std::string message);
	~AlertMessageModule();

	void Render();
};

#endif // MODULES_MESSAGE_H
