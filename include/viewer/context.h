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
 * @brief Main class of the app
 * 
 * Core of the app, used to connect most of the app's modules and to hold data
 * shared between them.
 */

class Context
{
public:
	/**
	 * @brief Constructs a new Context object
	 * 
	 * @param glslVersion Version of GLSL used by the project
	 */
	Context(std::string glslVersion);

	/**
	 * @brief Destroys the Context object
	 * 
	 */
	~Context();

	/**
	 * @brief Initializes various `ImGui` modules used by the application.
	 * 
	 */
	void Launch();

	/**
	 * @brief Initializes the application in `benchmark` mode.
	 * 
	 */
	void LaunchBenchmark();

	/**
	 * @brief Initializes the rendering window.
	 * 
	 */
	int Init();

	/**
	 * @brief Loads the various options of the app.
	 * 
	 * Both command line arguments and the TOML config file.
	 * 
	 * @param argc Number of arguments.
	 * @param argv List of arguments.
	 * @return int Returns 0 if successful, an error code otherwise.
	 */
	int LoadOptions(int argc, char** argv);

	/* Commands */

	/**
	 * @brief Creates a open PLY file selection dialog.
	 * 
	 */
	void CreateOpenPLYFileSelectionDialog();

	/**
	 * @brief Creates a save PLY file selection dialog.
	 * 
	 */
	void CreateSavePLYFileSelectionDialog();

	/**
	 * @brief Loads a PLY file.
	 * 
	 * @param filepath Path to the file to load.
	 */
	void LoadPLYFile(std::string filepath);

	/**
	 * @brief Moves the camera in trackball mode.
	 * 
	 * @param polarAngle Value of the polar angle.
	 * @param azimutalAngle Value of the azimutal angle.
	 */
	void MoveCamera(float polarAngle, float azimutalAngle);

	/**
	 * @brief Zooms the camera in or out.
	 * 
	 * @param intensity Zoom's intensity.
	 */
	void ZoomCamera(float intensity);

	/**
	 * @brief Reload the shaders.
	 * 
	 */
	void ReloadShaders();

	/**
	 * @brief Resets the list of paths to default materials.
	 * 
	 */
	void ResetDefaultMaterialsPaths();

	/**
	 * @brief Sets the shading type as simple shading.
	 * 
	 */
	void SetSimpleShading();

	/**
	 * @brief Sets the shading type as forward shading.
	 * 
	 */
	void SetForwardShading();

	/**
	 * @brief Toggles dark mode on or off.
	 * 
	 */
	void ToggleDarkMode();

	/**
	 * @brief Toggle's `ImGui`'s debug mode on or off.
	 * 
	 */
	void ToggleDebugMode();

	/**
	 * @brief Prepares for application exit.
	 * 
	 */
	void Quit();

	/**
	 * @brief Template class.
	 * 
	 * @tparam T
	 */
	template<class T>

	/**
	 * @brief Switches the visualiser to another renderer.
	 * 
	 */
	void SwitchRenderer();

	/* Debug commands */

	/**
	 * @brief Toggles the ImGui demo module on or off.
	 * 
	 */
	void ToggleImGuiDemoModule();

	/**
	 * @brief Toggles the mesh content module on or off.
	 * 
	 */
	void ToggleMeshContentModule();

	/**
	 * @brief Toggles the shaders content module on or off.
	 * 
	 */
	void ToggleShadersContentModule();

	/**
	 * @brief Toggles the FPS module on or off.
	 * 
	 */
	void ToggleImGuiFPSModule();

	/* Callbacks */

	/**
	 * @brief Processes all keyboard inputs.
	 * 
	 * @param key Last key caught.
	 * @param scancode Scan code of the last key caught.
	 * @param action State of the key (pressed, released, held).
	 * @param mods Whether Shift, Ctrl or Alt is pressed.
	 */
	void ProcessKeyboardInput(int key, int scancode, int action, int mods);

	/**
	 * @brief Processes all mouse movements.
	 * 
	 * @param x Mouse's X position.
	 * @param y Mouse's Y positio.
	 */
	void ProcessMouseMovement(double x, double y);

	/**
	 * @brief Processes all mouse clicks.
	 * 
	 * @param button Button which was pressed.
	 * @param action State of the button (pressed, released, held).
	 * @param mods Whether Shift, Ctrl or Alt is pressed.
	 */
	void ProcessMouseButton(int button, int action, int mods);

	/**
	 * @brief Processes all mouse wheel scrolls.
	 * 
	 * @param x X scroll.
	 * @param y Y scroll.
	 */
	void ProcessMouseScroll(double x, double y);

	/**
	 * @brief Get the new module ID.
	 * 
	 * @return int New module ID.
	 */
	int GetNewModuleID();

	/**
	 * @brief Signals the class that it needs to update.
	 * 
	 */
	void AskForUpdate();

	/**
	 * @brief Adds a module to the class.
	 * 
	 * @param module Module to be added.
	 */
	void AddModule(GUIModule* module);

	/* Getters & Setters */
	/**
	 * @brief Sets the mesh.
	 * 
	 * @param mesh Mesh to be set.
	 */
	void SetMesh(Mesh* mesh);


	/**
	 * @brief Sets the window's title.
	 * 
	 * @param title Title of the window.
	 */
	void SetWindowTitle(std::string title);

	/**
	 * @brief Sets the window's forced title.
	 * 
	 * @param title Title of the window.
	 */
	void SetForcedWindowTitle(std::string title);

	/**
	 * @brief Gets the window's title.
	 * 
	 * @return std::string The window's title.
	 */
	std::string GetWindowTitle();

	/**
	 * @brief Sets the window's size.
	 * 
	 * @param width Window's new width.
	 * @param height Window's new height.
	 */
	void SetWindowSize(int width, int height);

	/**
	 * @brief Gets the window's width.
	 * 
	 * @return int The window's width.
	 */
	int GetWindowWidth();

	/**
	 * @brief Gets the window's height.
	 * 
	 * @return int The window's height.
	 */
	int GetWindowHeight();

	/**
	 * @brief Sets the number of points lights in the scene.
	 * 
	 * @param nbPointLight Number of point lights.
	 */
	void SetPointLightNumber(int nbPointLight);

	/**
	 * @brief Gets the number of point lights in the scene.
	 * 
	 * @return int The number of point lights.
	 */
	int GetPointLightNumber();

	/**
	 * @brief Sets the app's current TOML config file to load.
	 * 
	 * @param file Path to the config file to load.
	 */
	void SetConfigFile(std::string file);

	/**
	 * @brief Gets the TOML config file's path.
	 * 
	 * @return std::string The path to the config file.
	 */
	std::string GetConfigFile();

	/**
	 * @brief Sets the path to the PLY input file.
	 * 
	 * @param file Path to the input file.
	 */
	void SetInputFile(std::string file);

	/**
	 * @brief Gets the path to the PLY input file.
	 * 
	 * @return std::string The path to the input file.
	 */
	std::string GetInputFile();

	/**
	 * @brief Sets whether the mesh must be sorted by material ID or not.
	 * 
	 * @param value Whether the mesh must be sorted by material ID or not.
	 */
	void SetForceUnsortedMesh(bool value);

	/**
	 * @brief Gets whether the mesh must be sorted by material ID or not.
	 * 
	 * @return true The mesh must be sorted by material ID.
	 * @return false The mesh will not be sorted.
	 */
	bool GetForceUnsortedMesh();

	/**
	 * @brief Sets benchmark mode.
	 * 
	 * @param benchmark Whether benchmark mode is on or not.
	 */
	void SetBenchmarkMode(bool benchmark);

	/**
	 * @brief Gets benchmark mode.
	 * 
	 * @return true Benchmark mode is on.
	 * @return false Benchmark mode is off.
	 */
	bool GetBenchmarkMode();

	/**
	 * @brief Sets `ImGui`'s debug mode.
	 * 
	 * @param debug Whether `InGui`'s bedug mode is on or off.
	 */
	void SetDebugMode(bool debug);

	/**
	 * @brief Gets `ImGui`'s debug mode.
	 * 
	 * @return true `ImGui`'s debug mode is on
	 * @return false `ImGui`'s debug mode is off
	 */
	bool GetDebugMode();

	/**
	 * @brief Sets whether the app is in dark mode or not.
	 * 
	 * @param darkMode Whether le app is in dark (true) or light (false) mode.
	 */
	void SetDarkMode(bool darkMode);

	/**
	 * @brief Gets whether the app is in dark mode or not.
	 * 
	 * @return true The app is in dark mode.
	 * @return false The app is in light mode.
	 */
	bool GetDarkMode();

	/**
	 * @brief Gets the command line data parser.
	 * 
	 * @return CLILoader Command line data parser.
	 */
	CLILoader GetCLI();

	/**
	 * @brief Gets the TOML config file parser.
	 * 
	 * @return TOMLLoader TOML config file parser.
	 */
	TOMLLoader GetTOML();

	/**
	 * @brief Gets the GLFWWindow object.
	 * 
	 * @return GLFWwindow* The GLFWWindow object.
	 */
	GLFWwindow* GetWindow();

private:
	/**
	 * @brief Render `ImGui`'s menu bar.
	 * 
	 */
	void RenderMenuBar();

	/**
	 * @brief Main rendering function of the app.
	 * 
	 */
	void Render();

	/**
	 * @brief Main update function of the app.
	 * 
	 */
	void Update();

	/**
	 * @brief Pointer to the GLFW window manager.
	 * 
	 */
	GLFWwindow* window = nullptr;

	/**
	 * @brief Window's title.
	 * 
	 */
	std::string windowTitle;

	/**
	 * @brief Window's title if forced by the config file or by a command line
	 * argument.
	 * 
	 */
	std::string windowTitleForced;

	/**
	 * @brief Current GLSL version in use.
	 * 
	 */
	std::string glslVersion;

	/**
	 * @brief Whether meshes are sorted by material ID when loaded or not.
	 * 
	 */
	bool forceUnsortedMeshMode = false;

	/**
	 * @brief Whether the app is in benchmark mode or not
	 * 
	 */
	bool benchmarkMode = false;

	/**
	 * @brief Whether `ImGui`'s debug mode is on or off.
	 * 
	 */
	bool debugMode = false;

	/**
	 * @brief Whether the app is in dark mode (true) or light mode (false).
	 * 
	 */
	bool darkMode = false;

	/**
	 * @brief Whether the mouse's left button has been pressed or not.
	 * 
	 */
	bool mouseLeftPressed = false;

	/**
	 * @brief Whether it's the first time the mouse has been clicked or not.
	 * 
	 */
	bool firstMouse = true;

	/**
	 * @brief ViewerModule used to display the application's mesh.
	 * 
	 */
	ViewerModule* viewer = nullptr;

	/**
	 * @brief PLY file reader.
	 * 
	 */
	PLYReader* reader = nullptr;

	/**
	 * @brief Scene object.
	 * 
	 */
	Scene* scene = nullptr;

	/**
	 * @brief List of paths to the various materials to use.
	 * 
	 */
	MaterialList* materialsPaths = nullptr;

	/**
	 * @brief List of active `ImGui` modules.
	 * 
	 */
	std::vector<GUIModule*> modules;

	/**
	 * @brief File browsing dialog.
	 * 
	 */
	imgui_addons::ImGuiFileBrowser* fileDialog = nullptr;

	/**
	 * @brief Mesh content module.
	 * 
	 */
	MeshContentModule* meshContent = nullptr;

	/**
	 * @brief Imgui Demo module.
	 * 
	 */
	ImGuiDemoModule* imguiDemo = nullptr;

	/**
	 * @brief ImGui FPS module.
	 * 
	 */
	ImGuiFPSModule* imguiFPS = nullptr;

	/**
	 * @brief Shaders content module.
	 * 
	 */
	ShadersContentModule* shadersContent = nullptr;

	/**
	 * @brief Path to the current config file.
	 * 
	 */
	std::string configFile = DATA_DIR "configs/default.toml";

	/**
	 * @brief Path to the PLY input file.
	 * 
	 */
	std::string inputFile;

	/**
	 * @brief ID of the next module to load.
	 * 
	 */
	int nextModuleID = 0;

	/**
	 * @brief Window's width.
	 * 
	 */
	int windowWidth = DEFAULT_WINDOW_WIDTH;

	/**
	 * @brief Window's height.
	 * 
	 */
	int windowHeight = DEFAULT_WINDOW_HEIGHT;

	/**
	 * @brief Defalt number of point lights in the scene.
	 * 
	 */
	int nbPointLight = DEFAULT_NB_POINT_LIGHT;

	/**
	 * @brief Delta time.
	 * 
	 */
	float deltaTime = 0.0;

	/**
	 * @brief Time since last frame.
	 * 
	 */
	float lastFrame = 0.0;

	/**
	 * @brief Time atthe beginning of the application.
	 * 
	 */
	float beginTime = 0.0;

	/**
	 * @brief Maximum time.
	 * 
	 */
	float maxTime = 10.0;

	/**
	 * @brief Frame count since the beginning of the application.
	 * 
	 */
	int frameCount = 0;

	/**
	 * @brief Command line parser.
	 * 
	 */
	CLILoader cli;

	/**
	 * @brief TOML config file parser.
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
