#include "context.h"

#include <iostream>

#include "CLI/CLI.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "toml.hpp"

#include "camera.h"
#include "filewithextension.h"
#include "modules/filedialog.h"
#include "modules/meshcontent.h"
#include "renderers/simple.h"
#include "scene.h"
#include "utils.h"

Context::Context(std::string glslVersion)
		: glslVersion(glslVersion) {}

Context::~Context() {
	/* Cleanup memory */

	for (auto i: this->modules)
		delete i;
	if (this->reader != nullptr)
		delete this->reader;
	if (this->fileDialog != nullptr)
		delete this->fileDialog;
	if (this->viewer != nullptr)
		delete this->viewer;
	if (this->scene != nullptr)
		delete this->scene;

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

	/* Initialize glbinding */

	glbinding::Binding::initialize(glfwGetProcAddress, false);
	// std::cout << "OpenGL version: "
	// 		<< glGetString(GL_VERSION) << std::endl;
	// std::cout << "GLSL version: "
	// 		<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	/* Initialize Dear ImGui */

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(this->glslVersion.c_str());

	// Setup ImGui style
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0;

	/* Create modules */

	this->scene = new Scene();
	this->viewer = new ViewerModule(this);
	this->viewer->GetRenderer()->SetScene(this->scene);

	this->fileDialog = nullptr;
	this->imguiDemo = nullptr;

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

	app.add_option("-i, --input", inputFile,
			"PLY file to load")
			->required()
			->check(CLI::ExistingFile)->check(FileWithExtension("ply"));
	app.add_option("-c, --config", configurationFile,
			"Configuration file")
			->check(CLI::ExistingFile)->check(FileWithExtension("toml"));
	app.add_option("--width", windowWidth,
			"Window’s width (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("--height", windowHeight,
			"Window’s height (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("-t, --title", windowTitle,
			"Window title");
	app.add_flag("-b, --benchmark", this->benchmarkMode,
			"Run the program in benchmark mode");
	app.add_flag("--debug", this->debugMode,
			"Enable debug mode (tools in menu bar)");
	app.add_flag("--dark", this->darkMode,
			"Enable dark mode");

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
	this->modules.push_back(
			new FileDialogModule(this, "Open file", ".ply", false));
}

void Context::LoadPLYFile(std::string filepath) {
	PLYReader* reader = new PLYReader(filepath);
	if (reader->Load()) {
		std::string filename = filepath.substr(filepath.rfind(PATH_DELIMITER) + 1);
		this->SetWindowTitle(filename);

		if (this->reader != nullptr)
			delete this->reader;
		this->reader = reader;

		if (this->meshContent != nullptr) {
			this->meshContent->Kill();
			this->meshContent =
					new MeshContentModule(this, filename, reader->GetMesh());
			this->modules.push_back((GUIModule*) this->meshContent);
		}

		this->SetMesh(reader->GetMesh());

		return;
	}
	delete reader;
	// TODO: Throw exception
}

void Context::MoveCamera(float polarAngle, float azimutalAngle) {
	if (this->scene != nullptr) {
		Camera* camera = this->scene->GetCamera();
		if (camera != nullptr) {
			camera->MoveCameraPolar(
					Eigen::Vector2f(polarAngle, azimutalAngle));
		}
	}
}

void Context::ZoomCamera(float intensity) {
	if (this->scene != nullptr) {
		Camera* camera = this->scene->GetCamera();
		if (camera != nullptr)
			camera->ZoomCameraPolar(intensity);
	}
}

void Context::ReloadShaders() {
	if (this->viewer != nullptr) {
		Renderer* renderer = this->viewer->GetRenderer();
		if (renderer != nullptr)
			renderer->ReloadShaders();
	}
}

void Context::ToggleDarkMode() {
	if (this->darkMode)
		this->SetLightMode();
	else
		this->SetDarkMode();
}

void Context::ToggleDebugMode() {
	this->debugMode = !this->debugMode;
}

void Context::Quit() {
	this->readyToDie = true;
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

void Context::ToggleMeshContentModule() {
	if (this->meshContent == nullptr) {
		std::string filename = this->reader->GetFilepath()
				.substr(this->reader->GetFilepath().rfind(PATH_DELIMITER) + 1);
		this->meshContent = new MeshContentModule(this, filename,
				reader->GetMesh());
		this->modules.push_back(this->meshContent);
	} else {
		this->meshContent->Kill();
		this->meshContent = nullptr;
	}
}

void Context::SetDarkMode() {
	ImGui::StyleColorsDark();
	windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);
	this->viewer->GetRenderer()
			->SetClearColor(Eigen::Vector4f(.1f, .1f, .1f, 1.f));
	this->darkMode = true;
}

void Context::SetLightMode() {
	ImGui::StyleColorsLight();
	windowClearColor = ImVec4(.95f, .95f, .95f, 1.f);
	this->viewer->GetRenderer()
			->SetClearColor(Eigen::Vector4f(1.f, 1.f, 1.f, 1.f));
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

void Context::ProcessKeyboardInput(int key, int scancode, int action,
		int mods) {
	// TODO: Find a way to handle non-QWERTY keyboards

	if (action == GLFW_PRESS) {
		if (mods == GLFW_MOD_CONTROL) {
			switch (key) {
				case GLFW_KEY_O:
					if (this->fileDialog == nullptr)
						this->CreateOpenPLYFileSelectionDialog();
					return;
				case GLFW_KEY_Q:
					this->Quit();
					return;
			}
		} else {
			switch (key) {
				case GLFW_KEY_R:
					this->ReloadShaders();
					return;
				case GLFW_KEY_TAB:
					this->showTools = !this->showTools;
					return;
			}
		}
	}

	/* Camera movements */
	{
		float movementSpeed = .1;

		if (mods == GLFW_MOD_SHIFT)
			movementSpeed *= 5;

		switch (key) {
			case GLFW_KEY_LEFT:
				// Move to the left
				this->MoveCamera(0., -movementSpeed);
				return;
			case GLFW_KEY_RIGHT:
				// Move to the right
				this->MoveCamera(0., movementSpeed);
				return;
			case GLFW_KEY_UP:
				// Move to the top
				this->MoveCamera(movementSpeed, 0.);
				return;
			case GLFW_KEY_DOWN:
				// Move to the bottom
				this->MoveCamera(-movementSpeed, 0.);
				return;
			case GLFW_KEY_O:
				// Zoom out
				this->ZoomCamera(-movementSpeed);
				return;
			case GLFW_KEY_P:
				// Zoom in
				this->ZoomCamera(movementSpeed);
				return;
		}
	}
}

void Context::ProcessMouseMovement(double x, double y) {}

void Context::ProcessMouseButton(int button, int action, int mods) {}

void Context::ProcessMouseScroll(double x, double y) {
	this->ZoomCamera(-y);
}

int Context::GetNewModuleID() {
	return this->nextModuleID++;
}

void Context::AskForUpdate() {
	this->needToUpdate = true;
}

void Context::SetMesh(Mesh* mesh) {
	if (this->scene != nullptr) {
		this->scene->SetMesh(mesh);
	} else {
		this->scene = new Scene(mesh);
		this->viewer->GetRenderer()->SetScene(this->scene);
	}
}

GLFWwindow* Context::GetWindow() {
	return this->window;
}

void Context::RenderMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open mesh...", "Ctrl+O"))
				this->CreateOpenPLYFileSelectionDialog();
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Ctrl+Q"))
				this->readyToDie = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			Renderer* renderer = this->viewer->GetRenderer();
			if (renderer != nullptr) {
				if (ImGui::BeginMenu("Render method")) {
					if (ImGui::MenuItem("Simple shading (no lights)", "",
							dynamic_cast<SimpleRenderer*>(renderer)))
						this->viewer->SetRenderer(new SimpleRenderer(renderer));
					// if (ImGui::MenuItem("Forward shading", "",
					// 		dynamic_cast<ForwardRenderer*>(renderer)))
					// 	this->viewer->SetRenderer(new SimpleRenderer(renderer));
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Reload shaders", "R"))
					this->ReloadShaders();
				ImGui::Separator();
			}
			if (ImGui::MenuItem("Show tools", "Tab", this->showTools))
				this->showTools = !this->showTools;
			if (ImGui::MenuItem("Enable dark mode", "", this->darkMode))
				this->ToggleDarkMode();
			if (ImGui::MenuItem("Enable debug menu", "", this->debugMode))
				this->ToggleDebugMode();
			ImGui::EndMenu();
		}
		if (this->debugMode) {
			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Show ImGui demo", "",
						(this->imguiDemo != nullptr)))
					this->ToggleImGuiDemoModule();
				if (ImGui::MenuItem("Show mesh content", "",
						(this->meshContent != nullptr)))
					this->ToggleMeshContentModule();
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}

void Context::Render() {
	if (this->needToUpdate)
		Update();

	if (this->showTools)
		this->RenderMenuBar();

	this->viewer->Render();

	if (this->showTools) {
		for (auto i: this->modules)
			i->Render();
	}
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
