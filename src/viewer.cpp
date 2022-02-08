#include <cmath>
#include <iostream>
#include <ctype.h>
#include <toml.hpp>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <plyreader.h>

#include <CLI/CLI.hpp>
#include <filewithextension.h>
#include <ImGuiFileBrowser.h>

#include "context.h"

// #define ENABLE_DARK_MODE
#define ENABLE_HIGH_DPI

#define ERR_GLFW		1
#define ERR_IMGUI		2

CLI::App app{"3D model viewer"};
GLFWwindow *window;
ImVec4 windowClearColor;
int windowSize[2] = { -1, -1 };
std::string windowTitle = std::string{};
bool isBenchmark = false;
std::string input_file, config_file = "../config.toml";
imgui_addons::ImGuiFileBrowser file_dialog;

Context* context;

const char *glslVersion;

static void PrintGLFWError(int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int InitializeGLFW() {
	glfwSetErrorCallback(PrintGLFWError);
	int err = glfwInit();
	if (err) {
#ifdef __APPLE__
		glslVersion = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef ENABLE_HIGH_DPI
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
#else
#ifdef _WIN32
		glslVersion = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
		glslVersion = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#endif
	}

	return err;
}

void CleanupGLFW() {
	glfwTerminate();
}

int InitializeImGui() {
	/* Setup Dear ImGui context */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	/* Setup Dear ImGui style */
#ifdef ENABLE_DARK_MODE
	ImGui::StyleColorsDark();
	windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);
#else
	ImGui::StyleColorsLight();
	windowClearColor = ImVec4(.95f, .95f, .95f, 1.f);
#endif

	/* Setup Platform/Renderer backends */
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	return 0;
}

void CleanupImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
}

void StartNewImGuiFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderImGuiFrame() {
	ImGui::Render();
	glfwGetFramebufferSize(window, &windowSize[0], &windowSize[1]);
	glViewport(0, 0, windowSize[0], windowSize[1]);
	glClearColor(windowClearColor.x * windowClearColor.w,
		windowClearColor.y * windowClearColor.w,
		windowClearColor.z * windowClearColor.w,
		windowClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	/* Swap frame buffers */
	glfwSwapBuffers(window);
}

int LoadCLI(int argc, char** argv) {
	// Loads the various command line options
	int windowWidth = -1, windowHeight = -1;
	app.add_option("-i,--input", input_file, "PLY file to load")->required()->check(CLI::ExistingFile)->check(FileWithExtension("ply"));
	app.add_option("-c,--config", config_file, "Config file to use")->check(CLI::ExistingFile)->check(FileWithExtension("toml"));;
	app.add_option("--width", windowWidth, "Window's width in pixels")->check(CLI::PositiveNumber);
	app.add_option("--height", windowHeight, "Window's height in pixels")->check(CLI::PositiveNumber);
	app.add_option("-t,--title", windowTitle, "3D viewer's window title");
	app.add_flag("-b,--benchmark", isBenchmark, "Run the program in benchmark mode");
	try {
		app.parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		return app.exit(e);
	}

	if (windowWidth != -1) windowSize[0] = windowWidth;
	if (windowHeight != -1) windowSize[1] = windowHeight;
	return 0;
}

void LoadTOML() {
	auto data = toml::parse(config_file);
	// TODO: Check if the file exists, check if each element exists

	auto& windowConfig = toml::find(data, "window");
	// Only load the TOML's data if the data hasn't been provided by the command line
	if (windowTitle.length() == 0) windowTitle = toml::find<std::string>(windowConfig, "title");
	if (windowSize[0] < 0) windowSize[0] = toml::find<int>(windowConfig, "windowWidth");
	if (windowSize[1] < 0) windowSize[1] = toml::find<int>(windowConfig, "windowHeight");

	auto& runConfig = toml::find(data, "run");
	if (!isBenchmark) isBenchmark = toml::find<bool>(runConfig, "isBenchmark");
}

// Default values if it hasn't been set by the command line nor the config file
void LoadDefault() {
	if (windowTitle.length() == 0) windowTitle = "3D Viewer";
	if (windowSize[0] < 0) windowSize[0] = 1024;
	if (windowSize[1] < 0) windowSize[1] = 800;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
		// TODO: Open file

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void showMainMenu(){
	bool open = false, save = false;
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("Open", NULL))
				open = true;
			if (ImGui::MenuItem("Save", NULL))
				save = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (open)
		ImGui::OpenPopup("Open File");
	if (save)
		ImGui::OpenPopup("Save File");

	// Only accept .ply file
	if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".ply")) {
		// TODO: pass the path to our program
		std::cout << file_dialog.selected_fn << std::endl;
		std::cout << file_dialog.selected_path << std::endl;
	}
	if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".ply")){
		// TODO: pass the path to our program
		std::cout << file_dialog.selected_fn << std::endl;
		std::cout << file_dialog.selected_path << std::endl;
		std::cout << file_dialog.ext << std::endl;
	}
}

int main(int argc, char** argv) {
	// Load the various levels of data (CLI, then TOML, then default)
	int ret = LoadCLI(argc, argv);
	if (ret) return ret;
	LoadTOML();
	LoadDefault();
	if (!InitializeGLFW())
		return ERR_GLFW;

	/* Create GLFW window */
	window = glfwCreateWindow(windowSize[0], windowSize[1], "3D Viewer", NULL, NULL);
	if (window == NULL)
		return ERR_GLFW;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Initialize ImGui context and style */
	if (InitializeImGui())
		return ERR_IMGUI;

	/* Create application context */
	context = new Context();

	context->LoadPLYFile(DATA_DIR "models/color_cube.ply");
	context->LoadPLYFile(DATA_DIR "models/matid_cube.ply");
	context->LoadPLYFile(DATA_DIR "models/basic_cube.ply");
	context->LoadPLYFile(DATA_DIR "models/gilet_union.ply");
	context->LoadPLYFile(DATA_DIR "models/doesntexists.ply");

	/* Main loop */
	while (!glfwWindowShouldClose(window)) {
		/* Poll latest events */
		// processInput(window);

		glfwPollEvents();
		StartNewImGuiFrame();

		context->Render();

		RenderImGuiFrame();
	}

	delete context;

	CleanupImGui();
	CleanupGLFW();

	return 0;
}
