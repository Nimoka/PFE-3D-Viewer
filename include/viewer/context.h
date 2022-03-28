#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuiFileBrowser.h>

#include "cliloader.h"
#include "material.h"
#include "tomlloader.h"
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

#define DEFAULT_DEF_MATERIAL	DATA_DIR "materials/mat_null.mat"
#define DEFAULT_NB_MATERIALS	7
#define DEFAULT_NB_POINT_LIGHT	1

#define ERROR_WINDOW_CREATION	2
#define ERROR_IMGUI_INIT		3
#define ERROR_CLI_PARSING		4
#define ERROR_CLI_MISS_TOML		5

#define MOUSE_SPEED				0.1
#define PI_DEGREE				180.0

/**
 * @brief Context class
 * Contains all the context necessary for rendering
 */

class Context
{
public:
	/**
	 * @brief Construct a new Context object
	 * 
	 * @param glslVersion , the version of glsl
	 */
	Context(std::string glslVersion);

	/**
	 * @brief Destroy the Context object
	 * 
	 */
	~Context();

	/**
	 * @brief languch the rendering
	 * 
	 */
	void Launch();

	/**
	 * @brief lauch in benchmarking mode
	 * 
	 */
	void LaunchBenchmark();

	/**
	 * @brief initialize the rendering window
	 * 
	 */
	/* Initialization */
	int Init();

	/**
	 * @brief load the options passing by command line
	 * 
	 * @param argc number of arguments
	 * @param argv vector of arguments
	 * @return int value of return , 0 if success
	 */
	int LoadOptions(int argc, char** argv);

	/* Commands */

	/**
	 * @brief Create a open PLY file selection dialog
	 * 
	 */
	void CreateOpenPLYFileSelectionDialog();

	/**
	 * @brief Create a save ply file selection dialog
	 * 
	 */
	void CreateSavePLYFileSelectionDialog();

	/**
	 * @brief load ply file
	 * 
	 * @param filepath the path of file
	 */
	void LoadPLYFile(std::string filepath);

	/**
	 * @brief move camera in trackball mode
	 * 
	 * @param polarAngle value of polar angle
	 * @param azimutalAngle value of azimutal angle
	 */
	void MoveCamera(float polarAngle, float azimutalAngle);

	/**
	 * @brief zoom the camera
	 * 
	 * @param intensity value of intensity
	 */
	void ZoomCamera(float intensity);

	/**
	 * @brief reload the shaders
	 * 
	 */
	void ReloadShaders();

	/**
	 * @brief reset default materials paths
	 * 
	 */
	void ResetDefaultMaterialsPaths();

	/**
	 * @brief set the simple shading
	 * 
	 */
	void SetSimpleShading();

	/**
	 * @brief set the forward shading
	 * 
	 */
	void SetForwardShading();

	/**
	 * @brief set the dark mode
	 * 
	 */
	void ToggleDarkMode();

	/**
	 * @brief set the debug mode in imgui
	 * 
	 */
	void ToggleDebugMode();

	/**
	 * @brief leave the rendering process
	 * 
	 */
	void Quit();

	/**
	 * @brief template class
	 * 
	 * @tparam T
	 */
	template<class T>

	/**
	 * @brief change to another renderer
	 * 
	 */
	void SwitchRenderer();

	/* Debug commands */

	/**
	 * @brief load and show the ImGui demo module
	 * 
	 */
	void ToggleImGuiDemoModule();

	/**
	 * @brief load and show the mesh content mudule
	 * 
	 */
	void ToggleMeshContentModule();

	/**
	 * @brief load and show the shaders content module
	 * 
	 */
	void ToggleShadersContentModule();

	/**
	 * @brief load and show the fps module
	 * 
	 */
	void ToggleImGuiFPSModule();

	/* Callbacks */

	/**
	 * @brief process the input key
	 * 
	 * @param key the key
	 * @param scancode  the key code
	 * @param action  the key action
	 * @param mods the key mods
	 */
	void ProcessKeyboardInput(int key, int scancode, int action, int mods);

	/**
	 * @brief process the mouse movement
	 * 
	 * @param x x position
	 * @param y y position
	 */
	void ProcessMouseMovement(double x, double y);

	/**
	 * @brief process the mouse button
	 * 
	 * @param button which button
	 * @param action button action
	 * @param mods button mods
	 */
	void ProcessMouseButton(int button, int action, int mods);

	/**
	 * @brief process mouse scroll
	 * 
	 * @param x x change
	 * @param y y change
	 */
	void ProcessMouseScroll(double x, double y);

	/**
	 * @brief get the new module ID
	 * 
	 * @return int the value of ID
	 */
	int GetNewModuleID();

	/**
	 * @brief ask for update
	 * 
	 */
	void AskForUpdate();

	/**
	 * @brief add module
	 * 
	 * @param module the module to be added
	 */
	void AddModule(GUIModule* module);

	/* Getters & Setters */
	/**
	 * @brief Set the mesh
	 * 
	 * @param mesh the mesh to be set up
	 */
	void SetMesh(Mesh* mesh);


	/**
	 * @brief Set the window title
	 * 
	 * @param title the title in window
	 */
	void SetWindowTitle(std::string title);

	/**
	 * @brief Set the forced window title
	 * 
	 * @param title the title to be set up
	 */
	void SetForcedWindowTitle(std::string title);

	/**
	 * @brief Get the window title
	 * 
	 * @return std::string  the title
	 */
	std::string GetWindowTitle();

	/**
	 * @brief Set the window size
	 * 
	 * @param width window width
	 * @param height window height
	 */
	void SetWindowSize(int width, int height);

	/**
	 * @brief wet the window width
	 * 
	 * @return int window width value
	 */
	int GetWindowWidth();

	/**
	 * @brief Get the window height
	 * 
	 * @return int window height value
	 */
	int GetWindowHeight();

	/**
	 * @brief Set the point light number
	 * 
	 * @param nbPointLight the number of point light
	 */
	void SetPointLightNumber(int nbPointLight);

	/**
	 * @brief Get the point light number
	 * 
	 * @return int the number of point light
	 */
	int GetPointLightNumber();

	/**
	 * @brief set the config file
	 * 
	 * @param file the file to be set up
	 */
	void SetConfigFile(std::string file);

	/**
	 * @brief Get the config file
	 * 
	 * @return std::string file name
	 */
	std::string GetConfigFile();

	/**
	 * @brief Set the input file
	 * 
	 * @param file the file name
	 */
	void SetInputFile(std::string file);

	/**
	 * @brief Get the input file
	 * 
	 * @return std::string file name
	 */
	std::string GetInputFile();

	/**
	 * @brief Set the force unsorted mesh
	 * 
	 * @param value the mode is on or not
	 */
	void SetForceUnsortedMesh(bool value);

	/**
	 * @brief Get the force unsorted mesh
	 * 
	 * @return true the mode is on
	 * @return false the mode is off
	 */
	bool GetForceUnsortedMesh();

	/**
	 * @brief Set the benchmark mode
	 * 
	 * @param benchmark the mode is on or not
	 */
	void SetBenchmarkMode(bool benchmark);

	/**
	 * @brief Get the benchmark mode
	 * 
	 * @return true mode is on
	 * @return false mode is off
	 */
	bool GetBenchmarkMode();

	/**
	 * @brief Set the debug mode object
	 * 
	 * @param debug the mode is on or not
	 */
	void SetDebugMode(bool debug);

	/**
	 * @brief Get the debug mode
	 * 
	 * @return true the mode is on
	 * @return false the mode is off
	 */
	bool GetDebugMode();

	/**
	 * @brief Set the dark mode object
	 * 
	 * @param darkMode the mode is on or not
	 */
	void SetDarkMode(bool darkMode);

	/**
	 * @brief Get the dark mode
	 * 
	 * @return true the mode is on
	 * @return false the mode is off
	 */
	bool GetDarkMode();

	/**
	 * @brief load the command line interface
	 * 
	 * @return CLILoader command line interface
	 */
	CLILoader GetCLI();

	/**
	 * @brief load the toml
	 * 
	 * @return TOMLLoader
	 */
	TOMLLoader GetTOML();

	/**
	 * @brief Get the window object
	 * 
	 * @return GLFWwindow* the window
	 */
	GLFWwindow* GetWindow();

private:
	/**
	 * @brief render the imgui menu bar
	 * 
	 */
	void RenderMenuBar();

	/**
	 * @brief rendering
	 * 
	 */
	void Render();

	/**
	 * @brief update
	 * 
	 */
	void Update();

	/**
	 * @brief window initialized as null
	 * 
	 */
	GLFWwindow* window = nullptr;

	/**
	 * @brief the title of window
	 * 
	 */
	std::string windowTitle;

	/**
	 * @brief the forced title
	 * 
	 */
	std::string windowTitleForced;

	/**
	 * @brief the glsl version
	 * 
	 */
	std::string glslVersion;

	/**
	 * @brief force unsrted mesh mode off
	 * 
	 */
	bool forceUnsortedMeshMode = false;

	/**
	 * @brief benchmark mode off
	 * 
	 */
	bool benchmarkMode = false;

	/**
	 * @brief debug mode off
	 * 
	 */
	bool debugMode = false;

	/**
	 * @brief dark mode off
	 * 
	 */
	bool darkMode = false;

	/**
	 * @brief mouse not pressed
	 * 
	 */
	bool mouseLeftPressed = false;

	/**
	 * @brief is the first time that the mouse is clicked
	 * 
	 */
	bool firstMouse = true;

	/**
	 * @brief initialize viewer
	 * 
	 */
	ViewerModule* viewer = nullptr;

	/**
	 * @brief initialize reader
	 * 
	 */
	PLYReader* reader = nullptr;

	/**
	 * @brief initialize scene
	 * 
	 */
	Scene* scene = nullptr;

	/**
	 * @brief initialize materialsPath
	 * 
	 */
	MaterialList* materialsPaths = nullptr;

	/**
	 * @brief initialize models vector
	 * 
	 */
	std::vector<GUIModule*> modules;

	/**
	 * @brief initialze filedialoger
	 * 
	 */
	imgui_addons::ImGuiFileBrowser* fileDialog = nullptr;

	/**
	 * @brief initialize mesh content
	 * 
	 */
	MeshContentModule* meshContent = nullptr;

	/**
	 * @brief initialize imguiDemo module
	 * 
	 */
	ImGuiDemoModule* imguiDemo = nullptr;

	/**
	 * @brief initializw imguiFPS module
	 * 
	 */
	ImGuiFPSModule* imguiFPS = nullptr;

	/**
	 * @brief initialize shader content module
	 * 
	 */
	ShadersContentModule* shadersContent = nullptr;

	/**
	 * @brief initialize config file path
	 * 
	 */
	std::string configFile = DATA_DIR "configs/default.toml";

	/**
	 * @brief declare a input file name
	 * 
	 */
	std::string inputFile;

	/**
	 * @brief set next module ID as 0
	 * 
	 */
	int nextModuleID = 0;

	/**
	 * @brief set windows width as default
	 * 
	 */
	int windowWidth = DEFAULT_WINDOW_WIDTH;

	/**
	 * @brief set windows height as default
	 * 
	 */
	int windowHeight = DEFAULT_WINDOW_HEIGHT;

	/**
	 * @brief set number of point lights as default
	 * 
	 */
	int nbPointLight = DEFAULT_NB_POINT_LIGHT;

	/**
	 * @brief initialize delta time
	 * 
	 */
	float deltaTime = 0.0;

	/**
	 * @brief initialize last frame time
	 * 
	 */
	float lastFrame = 0.0;

	/**
	 * @brief initialize begin time
	 * 
	 */
	float beginTime = 0.0;

	/**
	 * @brief initialize max time
	 * 
	 */
	float maxTime = 10.0;

	/**
	 * @brief initilizw frame count
	 * 
	 */
	int frameCount = 0;

	/**
	 * @brief declare command line interface
	 * 
	 */
	CLILoader cli;

	/**
	 * @brief declare toml
	 * 
	 */
	TOMLLoader toml;

	/**
	 * @brief initialize show tools
	 * 
	 */
	bool showTools = true;

	/**
	 * @brief initlialize needToUpdate
	 * 
	 */
	bool needToUpdate = false;

	/**
	 * @brief initialize readyToDie
	 * 
	 */
	bool readyToDie = false;

	/**
	 * @brief declare window clear color
	 * 
	 */
	ImVec4 windowClearColor;
};

#endif // CONTEXT_H
