#include "modules/message.h"

#include <iostream>
#include <string>

#include "imgui_addons.h"

MessageModule::MessageModule(void* context, std::string message)
		: GUIModule(context)
		, message(message) {
	this->Init();
}

MessageModule::~MessageModule() {}

void MessageModule::Render() {
	if (ImGui::Begin(std::string("Message###"
			+ std::to_string(this->id)).c_str(), nullptr, this->flags)) {
		ImGui::Text("%s", this->message.c_str());
	}
	ImGui::End();
}

std::string MessageModule::GetMessage() {
	return this->message;
}

void MessageModule::Init() {
	this->flags = ImGuiWindowFlags_None
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoCollapse;
}

LoadingMessageModule::LoadingMessageModule(void* context, std::string message)
		: MessageModule(context, message) {
	this->accentColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
	this->backgroundColor = ImGui::GetColorU32(ImGuiCol_Button);
}

LoadingMessageModule::~LoadingMessageModule() {}

void LoadingMessageModule::Render() {
	if (ImGui::Begin(std::string("Loading###"
			+ std::to_string(this->id)).c_str(), nullptr, this->flags)) {
		ImGui::Spinner("spinner", 6, 2, this->accentColor);
		ImGui::SameLine();
		ImGui::Text("%s", this->message.c_str());
	}
	ImGui::End();
}

ProcessingMessageModule::ProcessingMessageModule(void* context,
		std::string message, int* currentValue, int* expectedValue)
		: MessageModule(context, message)
		, currentValue(currentValue)
		, expectedValue(expectedValue) {
	this->accentColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
	this->backgroundColor = ImGui::GetColorU32(ImGuiCol_Button);
}

ProcessingMessageModule::~ProcessingMessageModule() {}

void ProcessingMessageModule::Render() {
	if (ImGui::Begin(std::string("Processing###"
			+ std::to_string(this->id)).c_str(), nullptr, this->flags)) {
		float progression = (*this->currentValue)
				/ ((*this->expectedValue) * 1.);
		if (progression < 0.)
			progression = 0.;
		else if (progression > 1.)
			progression = 1.;

		ImGui::Text("%s", this->message.c_str());
		ImGui::ProgressBar("progress", progression,
				ImVec2(320, 6), this->backgroundColor, this->accentColor);
	}
	ImGui::End();
}

AlertMessageModule::AlertMessageModule(void* context, std::string message)
		: MessageModule(context, message) {}

AlertMessageModule::~AlertMessageModule() {}

void AlertMessageModule::Render() {
	if (ImGui::Begin(std::string("Alert###"
			+ std::to_string(this->id)).c_str(), nullptr, this->flags)) {
		ImGui::Text("%s", this->message.c_str());
		ImGui::PushItemWidth(-(ImGui::GetWindowContentRegionWidth()
				- ImGui::CalcItemWidth()));
		if (ImGui::Button("OK", ImVec2(64., 0.)))
			this->Kill();
	}
	ImGui::End();
}
