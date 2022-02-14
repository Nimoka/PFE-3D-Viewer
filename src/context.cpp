#include "context.h"

#include <iostream>

#include "CLI/CLI.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "toml.hpp"

#include "filewithextension.h"
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
	this->SetLightMode();

	return 0;
}

int Context::LoadCLIContext(int argc, char** argv) {
	/* Create CLI context */

	CLI::App app{DEFAULT_WINDOW_TITLE};

	std::string inputFile, configurationFile;
	std::string windowTitle;
	int windowWidth = 0, windowHeight = 0;

	/* Set CLI options */

	app.add_option("-i, --input", inputFile, "PLY file to load")
			->required()
			->check(CLI::ExistingFile)->check(FileWithExtension("ply"));
	app.add_option("-c, --config", configurationFile, "Configuration file")
			->check(CLI::ExistingFile)->check(FileWithExtension("toml"));
	app.add_option("--width", windowWidth, "Window’s width (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("--height", windowHeight, "Window’s height (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("-t, --title", windowTitle, "Window title");
	app.add_flag("-b, --benchmark", this->benchmarkMode, "Run the program in benchmark mode");
	app.add_flag("--debug", this->debugMode, "Enable debug mode (tools in menu bar)");
	app.add_flag("--dark", this->darkMode, "Enable dark mode");

	/* Parse CLI options */

	try {
		app.parse(argc, argv);
	} catch (const CLI::ParseError &e) {
		return app.exit(e);
	}

	/* Load default configuration file */

	this->LoadTOMLContext(DATA_DIR "configs/default.toml");
	// TODO: Maybe consider having two differents default configuration files:
	//       one for viewer mode and another for benchmark mode, and so load
	//       different files depending on `this->benchmarkMode`.
	// Yeah, I moved it here instead of in `Init()`, so it can be done without
	// calling `LoadCLIContext` before `LoadDefaultContext` just for knowing
	// the mode… In fact, both functions can be fusionned.

	/* Process CLI options found (if needed) */

	// Configuration file
	if (!configurationFile.empty()) {
		if (!this->LoadTOMLContext(configurationFile))
			return ERROR_CLI_MISS_TOML;
	}

	// Window’s title
	if (!windowTitle.empty())
		this->SetForcedWindowTitle(windowTitle);

	// Window’s size
	if (windowWidth && windowHeight) {
		this->SetWindowSize(windowWidth, windowHeight);
	} else if (windowWidth) {
		this->SetWindowSize(windowWidth, DEFAULT_WINDOW_HEIGHT);
	} else if (windowHeight) {
		this->SetWindowSize(DEFAULT_WINDOW_WIDTH, windowHeight);
	}

	// Window’s style
	if (this->darkMode)
		this->SetDarkMode();

	// PLY file to load
	if (!inputFile.empty())
		this->LoadPLYFile(inputFile);

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

void Context::SetDarkMode() {
	ImGui::StyleColorsDark();
	windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);
	this->darkMode = true;
}

void Context::SetLightMode() {
	ImGui::StyleColorsLight();
	windowClearColor = ImVec4(.95f, .95f, .95f, 1.f);
	this->darkMode = false;
}

void Context::SetWindowTitle(std::string title) {
	if (this->windowTitleForced.empty()) {
		std::string formattedTitle = title + " — " + DEFAULT_WINDOW_TITLE;
		glfwSetWindowTitle(this->window, formattedTitle.c_str());
	}
}

void Context::SetForcedWindowTitle(std::string title) {
	this->windowTitleForced = title;
	glfwSetWindowTitle(this->window, title.c_str());
}

void Context::SetWindowSize(int width, int height) {
	glfwSetWindowSize(this->window, width, height);
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

void Context::KillFileDialogPlease() {
	if (this->fileDialog != nullptr)
		delete this->fileDialog;
}

void Context::AskForUpdate() {
	this->needToUpdate = true;
}

GLFWwindow* Context::GetWindow() {
	return this->window;
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
		ImGui::EndMainMenuBar();
	}
}
