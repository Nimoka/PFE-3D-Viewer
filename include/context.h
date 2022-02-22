#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ImGuiFileBrowser.h"

#include "modules/imguidemo.h"
#include "modules/module.h"
#include "modules/viewer.h"
#include "plyreader.h"

#define DEFAULT_WINDOW_TITLE	"3D Viewer"
#define DEFAULT_WINDOW_WIDTH	1280
#define DEFAULT_WINDOW_HEIGHT	800

#define ERROR_WINDOW_CREATION	2
#define ERROR_IMGUI_INIT		3
#define ERROR_CLI_PARSING		4
#define ERROR_CLI_MISS_TOML		5

class Context
{
public:
	Context(std::string glslVersion);
	~Context();

	void Launch();

	/* Initialization */
	int Init();
	int LoadCLIContext(int argc, char** argv);
	bool LoadTOMLContext(std::string filepath);

	/* Commands */
	void CreateOpenPLYFileSelectionDialog();
	void CreateSavePLYFileSelectionDialog();
	void LoadPLYFile(std::string filepath);
	void MoveCamera(float polarAngle, float azimutalAngle);
	void ZoomCamera(float intensity);
	void ReloadShaders();
	void Quit();

	/* Debug commands */
	void ToggleImGuiDemoModule();

	/* Customization */
	void SetDarkMode();
	void SetLightMode();
	void SetWindowTitle(std::string title);
	void SetForcedWindowTitle(std::string title);
	void SetWindowSize(int width, int height);

	/* Callbacks */
	void ProcessKeyboardInput(int key, int scancode, int action, int mods);
	void ProcessMouseMovement(double x, double y);
	void ProcessMouseButton(int button, int action, int mods);
	void ProcessMouseScroll(double x, double y);

	int GetNewModuleID();
	void AskForUpdate();

	GLFWwindow* GetWindow();

private:
	void RenderMenuBar();

	void Render();
	void Update();

	GLFWwindow* window;
	std::string windowTitleForced;
	std::string glslVersion;

	bool benchmarkMode = false;
	bool debugMode = false;
	bool darkMode = false;

	ViewerModule* viewer;
	std::vector<GUIModule*> modules;
	std::vector<PLYReader*> readers;
	imgui_addons::ImGuiFileBrowser* fileDialog;

	ImGuiDemoModule* imguiDemo;

	int nextModuleID = 0;

	bool showTools = true;

	bool needToUpdate = false;
	bool readyToDie = false;

	ImVec4 windowClearColor;
};

#endif // CONTEXT_H
