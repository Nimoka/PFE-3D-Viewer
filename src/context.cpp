#include "context.h"

#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "toml.hpp"

#include "modules/filedialog.h"
#include "modules/plycontent.h"

Context::Context(std::string glslVersion) {
	this->glslVersion = glslVersion;
	this->readyToDie = false;
}

Context::~Context() {
	/* Cleanup memory */

	for (auto i: this->modules)
		delete i;
	for (auto i: this->readers)
		delete i;
	if (this->fileDialog != nullptr)
		delete this->fileDialog;
	if (this->viewer != nullptr)
		delete this->viewer;

	/* Cleanup ImGui */

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(this->window);
}

void Context::Launch() {
	int windowWidth, windowHeight;
	while (!glfwWindowShouldClose(window) && !this->readyToDie) {
		/* Poll latest events */

		glfwPollEvents();

		/* Start new ImGui frame */

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		/* Set ImGui rendering */

		this->Render();

		/* Render ImGui frame */

		ImGui::Render();
		glfwGetFramebufferSize(this->window, &windowWidth, &windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(this->windowClearColor.x * this->windowClearColor.w,
				this->windowClearColor.y * this->windowClearColor.w,
				this->windowClearColor.z * this->windowClearColor.w,
				this->windowClearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap frame buffers */

		glfwSwapBuffers(window);
	}
}

int Context::Init() {
	/* Create GLFW window */

	this->window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
			DEFAULT_WINDOW_TITLE, NULL, NULL);
	if (this->window == NULL)
		return ERROR_WINDOW_CREATION;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Initialize Dear ImGui */

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(this->glslVersion.c_str());

	/* Load default values (hard-coded) */

	this->benchmarkMode = false;
	this->debugMode = false;
	this->SetDarkMode(false);

	/* Create modules */

	this->viewer = new ViewerModule(this);
	this->fileDialog = nullptr;
	this->imguiDemo = nullptr;

	return 0;
}

int Context::LoadOptions(int argc, char** argv) {
	/* Fetch original config file */
	LoadTOMLContext(configFile);

	// Window’s title
	if (!windowTitle.empty())
		SetForcedWindowTitle(windowTitle);

	// Window’s size
	SetWindowSize(windowWidth, windowHeight);

	// Window’s style
	SetDarkMode(darkMode);

	/* Fetch CLI options (and the designated extra config file) */
	int res = cli.LoadContext(this, argc, argv);
	if (res)
		return res;

	// PLY file to load
	if (!inputFile.empty())
		LoadPLYFile(inputFile);

	return 0;
}

bool Context::LoadTOMLContext(std::string filepath) {
	try {
		/* Parse TOML file */

		auto data = toml::parse(filepath);

		/* Process options found */

		// Window
		{
			auto& window = toml::find(data, "window");

			std::string title = toml::find_or<std::string>(window, "title", "");
			if (!title.empty())
				this->SetForcedWindowTitle(title);

			int width = toml::find_or<int>(window, "width", 0);
			int height = toml::find_or<int>(window, "height", 0);
			if (width && height) {
				this->SetWindowSize(width, height);
			} else if (width) {
				this->SetWindowSize(width, DEFAULT_WINDOW_HEIGHT);
			} else if (height) {
				this->SetWindowSize(DEFAULT_WINDOW_WIDTH, height);
			}
		}
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch (const toml::syntax_error &e) {
		std::cerr << "Syntax error found in file ‘" << filepath << "’.";
		return false;
	} catch (...) {
		return false;
	}

	return true;
}

void Context::CreateOpenPLYFileSelectionDialog() {
	this->fileDialog = new imgui_addons::ImGuiFileBrowser();
	this->modules.push_back(new FileDialogModule(this, "Open file", ".ply", false));
}

// void Context::CreateSavePLYFileSelectionDialog() {
// 	this->modules.push_back(new FileDialogModule(this, "Save file", ".ply", true));
// }

void Context::LoadPLYFile(std::string filepath) {
	PLYReader* reader = new PLYReader(filepath);
	if (reader->Load()) {
		std::string filename = filepath.substr(filepath.rfind('/') + 1);
		this->SetWindowTitle(filename);
		this->modules.push_back(new PLYContentModule(this, filename, reader->GetMesh()));
		this->readers.push_back(reader);
		return;
	}
	delete reader;
	// TODO: Throw exception
}

void Context::ToggleImGuiDemoModule() {
	if (this->imguiDemo == nullptr) {
		this->imguiDemo = new ImGuiDemoModule(this);
		this->modules.push_back(this->imguiDemo);
	} else {
		this->imguiDemo->Kill();
		this->imguiDemo = nullptr;
	}
}

void Context::SetWindowTitle(std::string title) {
	if (this->windowTitleForced.empty()) {
		this->windowTitle = title + " — " + DEFAULT_WINDOW_TITLE;
		glfwSetWindowTitle(this->window, this->windowTitle.c_str());
	}
}

void Context::SetForcedWindowTitle(std::string title) {
	this->windowTitleForced = title;
	glfwSetWindowTitle(this->window, title.c_str());
}

std::string Context::GetWindowTitle() {
	return this->windowTitleForced.empty() ? this->windowTitleForced : this->windowTitle;
}

void Context::SetWindowSize(int width, int height) {
	glfwSetWindowSize(this->window, width, height);
	this->windowWidth = width;
	this->windowHeight = height;
}

int Context::GetWindowWidth() {
	return this->windowWidth;
}

int Context::GetwindowHeight() {
	return this->windowHeight;
}

void Context::ProcessKeyboardInput(int key, int scancode, int action, int mods) {
	if (this->fileDialog == nullptr) {
		if (mods == GLFW_MOD_CONTROL && key == GLFW_KEY_O && action == GLFW_PRESS) {
			this->CreateOpenPLYFileSelectionDialog();
			return;
		}
	}
}

void Context::ProcessMouseMovement(double x, double y) {}

void Context::ProcessMouseButton(int button, int action, int mods) {}

void Context::ProcessMouseScroll(double x, double y) {}

void Context::Render() {
	this->RenderMenuBar();

	this->viewer->Render();

	for (auto i: this->modules)
		i->Render();

	if (this->needToUpdate)
		Update();
}

void Context::Update() {
	for (auto it = this->modules.begin(); it != this->modules.end();) {
		if ((*it)->IsReadyToDie()) {
			delete *it;
			it = this->modules.erase(it);
		} else {
			it++;
		}
	}
	this->needToUpdate = false;
}

int Context::GetNewModuleID() {
	return this->nextModuleID++;
}

void Context::AskForUpdate() {
	this->needToUpdate = true;
}

GLFWwindow* Context::GetWindow() {
	return this->window;
}


void Context::SetBenchmarkMode(bool benchmark) {
	this->benchmarkMode = benchmark;
}
bool Context::GetBenchmarkMode() {
	return this->benchmarkMode;
}

void Context::SetDebugMode(bool debug) {
	this->debugMode = debug;
}
bool Context::GetDarkMode() {
	return this->darkMode;
}

void Context::SetDarkMode(bool dark) {
	if (dark) {
		ImGui::StyleColorsDark();
		windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);
	} else {
		ImGui::StyleColorsLight();
		windowClearColor = ImVec4(.95f, .95f, .95f, 1.f);
	}
	this->darkMode = dark;
}
bool Context::GetDebugMode() {
	return this->debugMode;
}

void Context::SetConfigFile(std::string file) {
	this->configFile = file;
}
std::string Context::GetConfigFile() {
	return this->configFile;
}

void Context::SetInputFile(std::string file) {
	this->inputFile = file;
}
std::string Context::GetInputFile() {
	return this->inputFile;
}

void Context::RenderMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				this->CreateOpenPLYFileSelectionDialog();
			// if (ImGui::MenuItem("Save...", "Ctrl+W"))
			// 	this->CreateSavePLYFileSelectionDialog();
			if (ImGui::MenuItem("Quit", "Ctrl+Q"))
				this->readyToDie = true;
			ImGui::EndMenu();
		}
		if (this->debugMode) {
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Show ImGui demo", "", (this->imguiDemo != nullptr)))
					this->ToggleImGuiDemoModule();
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}
