#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuiFileBrowser.h>

#include "cliloader.h"
#include "modules/imguidemo.h"
#include "modules/imguiFPS.h"
#include "modules/meshcontent.h"
#include "modules/module.h"
#include "modules/shaderscontent.h"
#include "modules/viewer.h"
#include "plyreader.h"

#define DEFAULT_WINDOW_TITLE	"3D Viewer"
#define DEFAULT_WINDOW_WIDTH	1280
#define DEFAULT_WINDOW_HEIGHT	800

#define ERROR_WINDOW_CREATION	2
#define ERROR_IMGUI_INIT		3
#define ERROR_CLI_PARSING		4
#define ERROR_CLI_MISS_TOML		5

#define MOUSE_SPEED				0.1
#define PI_DEGREE				180.0


class Context
{
public:
	Context(std::string glslVersion);
	~Context();

	void Launch();

	/* Initialization */
	int Init();
	int LoadOptions(int argc, char** argv);
	bool LoadTOMLContext(std::string filepath);

	/* Commands */
	void CreateOpenPLYFileSelectionDialog();
	void CreateSavePLYFileSelectionDialog();
	void LoadPLYFile(std::string filepath);
	void MoveCamera(float polarAngle, float azimutalAngle);
	void ZoomCamera(float intensity);
	void ReloadShaders();
	void ToggleDarkMode();
	void ToggleDebugMode();
	void Quit();

	template<class T>
	void SwitchRenderer();

	/* Debug commands */
	void ToggleImGuiDemoModule();
	void ToggleMeshContentModule();
	void ToggleShadersContentModule();
	void ToggleImGuiFPSModule();

	/* Callbacks */
	void ProcessKeyboardInput(int key, int scancode, int action, int mods);
	void ProcessMouseMovement(double x, double y);
	void ProcessMouseButton(int button, int action, int mods);
	void ProcessMouseScroll(double x, double y);

	int GetNewModuleID();
	void AskForUpdate();

	void AddModule(GUIModule* module);

	/* Getters & Setters */
	void SetMesh(Mesh* mesh);

	void SetWindowTitle(std::string title);
	void SetForcedWindowTitle(std::string title);
	std::string GetWindowTitle();

	void SetWindowSize(int width, int height);
	int GetWindowWidth();
	int GetWindowHeight();

	void SetConfigFile(std::string file);
	std::string GetConfigFile();
	void SetInputFile(std::string file);
	std::string GetInputFile();

	void SetBenchmarkMode(bool benchmark);
	bool GetBenchmarkMode();
	void SetDebugMode(bool debug);
	bool GetDebugMode();
	void SetDarkMode(bool darkMode);
	bool GetDarkMode();

	CLILoader GetCLI();

	GLFWwindow* GetWindow();

private:
	void RenderMenuBar();

	void Render();
	void Update();

	GLFWwindow* window = nullptr;
	std::string windowTitle;
	std::string windowTitleForced;
	std::string glslVersion;

	bool benchmarkMode = false;
	bool debugMode = false;
	bool darkMode = false;
	bool mouseLeftPressed = false;
	bool firstMouse = true;

	ViewerModule* viewer = nullptr;
	PLYReader* reader = nullptr;
	Scene* scene = nullptr;

	std::vector<GUIModule*> modules;

	imgui_addons::ImGuiFileBrowser* fileDialog = nullptr;
	MeshContentModule* meshContent = nullptr;
	ImGuiDemoModule* imguiDemo = nullptr;
	ImGuiFPSModule* imguiFPS = nullptr;
	ShadersContentModule* shadersContent = nullptr;

	std::string configFile = DATA_DIR "configs/default.toml";
	std::string inputFile;

	int nextModuleID = 0;
	int windowWidth = DEFAULT_WINDOW_WIDTH;
	int windowHeight = DEFAULT_WINDOW_HEIGHT;

	CLILoader cli;

	bool showTools = true;

	bool needToUpdate = false;
	bool readyToDie = false;

	ImVec4 windowClearColor;
};

#endif // CONTEXT_H
