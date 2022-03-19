#include "context.h"

#include <Eigen/Geometry>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "camera.h"
#include "filewithextension.h"
#include "modules/filedialog.h"
#include "modules/message.h"
#include "renderers/forward.h"
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

	//glfwSwapInterval(0);
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
#ifdef DEBUG_OPENGL_CONTEXT
	std::cout << "[DEBUG_OPENGL_CONTEXT] OpenGL version: "
			<< glGetString(GL_VERSION) << std::endl;
	std::cout << "[DEBUG_OPENGL_CONTEXT] GLSL version: "
			<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	{
		int data = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &data);
		std::cout << "[DEBUG_OPENGL_CONTEXT] Max uniform block size: "
				<< data << " bytes" << std::endl;
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &data);
		std::cout << "[DEBUG_OPENGL_CONTEXT] Max texture buffer size: "
				<< data << " bytes" << std::endl;
	}

	glbinding::aux::enableGetErrorCallback();
#endif

	/* Initialize OpenGL */

	glEnable(GL_CULL_FACE);

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
	this->SetDarkMode(false);

	return 0;
}

int Context::LoadOptions(int argc, char** argv) {
	/* Fetch original config file */
	toml.LoadContext(this, configFile);

	// Window’s title
	if (!windowTitle.empty())
		SetForcedWindowTitle(windowTitle);

	// Window’s size
	SetWindowSize(windowWidth, windowHeight);

	/* Fetch CLI options (and the designated extra config file) */
	int res = cli.LoadContext(this, argc, argv);
	if (res)
		return res;

	// PLY file to load
	if (inputFile.empty())
		this->CreateOpenPLYFileSelectionDialog();
	else
		this->LoadPLYFile(inputFile);

	return 0;
}

void Context::CreateOpenPLYFileSelectionDialog() {
	this->fileDialog = new imgui_addons::ImGuiFileBrowser();
	this->AddModule(
			new FileDialogModule(this, "Open file", ".ply", false));
}

void Context::LoadPLYFile(std::string filepath) {
	PLYReader* reader = new PLYReader(this, filepath);
	if (reader->Load()) {
		std::string filename = filepath.substr(
				filepath.rfind(PATH_DELIMITER) + 1);
		this->SetWindowTitle(filename);

		if (this->reader != nullptr)
			delete this->reader;
		this->reader = reader;

		if (this->meshContent != nullptr) {
			this->meshContent->Kill();
			this->meshContent =
					new MeshContentModule(this, filename, reader->GetMesh());
			this->AddModule((GUIModule*) this->meshContent);
		}

		this->SetMesh(reader->GetMesh());
	} else {
		this->AddModule(new AlertMessageModule(this,
				"Failed to load file '" + filepath + "'."));
		delete reader;
	}
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
	this->SetDarkMode(!this->darkMode);
}

void Context::ToggleDebugMode() {
	this->debugMode = !this->debugMode;
}

void Context::Quit() {
	this->readyToDie = true;
}

template <class T>
void Context::SwitchRenderer() {
	Renderer* renderer = this->viewer->GetRenderer();
	if (renderer == nullptr)
		return;

	renderer = new T(renderer);
	this->viewer->SetRenderer(renderer);
	if (this->shadersContent != nullptr) {
		this->shadersContent->SetShaders(
				renderer->GetNbShaders(), renderer->GetShaders());
	}
}

void Context::ToggleImGuiDemoModule() {
	if (this->imguiDemo == nullptr) {
		this->imguiDemo = new ImGuiDemoModule(this);
		this->AddModule(this->imguiDemo);
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
		this->AddModule(this->meshContent);
	} else {
		this->meshContent->Kill();
		this->meshContent = nullptr;
	}
}

void Context::ToggleShadersContentModule() {
	if (this->shadersContent == nullptr) {
		Renderer* renderer = this->viewer->GetRenderer();
		this->shadersContent = new ShadersContentModule(this,
				renderer->GetNbShaders(),
				renderer->GetShaders());
		this->AddModule(this->shadersContent);
	} else {
		this->shadersContent->Kill();
		this->shadersContent = nullptr;
	}
}

void Context::ToggleImGuiFPSModule() {
	if (this->imguiFPS == nullptr) {
		this->imguiFPS = new ImGuiFPSModule(this);
		this->modules.push_back(this->imguiFPS);
	} else {
		this->imguiFPS->Kill();
		this->imguiFPS = nullptr;
	}
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
				case GLFW_KEY_L:
					if (this->scene != nullptr)
						this->scene->AddDirectionalLight(
								new DirectionalLight(
										Eigen::Vector3f(.2, .2, .2),
										Eigen::Vector3f(1., 1., 0.)));
					return;
				case GLFW_KEY_Y:
					if (this->scene != nullptr)
						this->scene->AddRandomPointLight(
							new PointLight(Eigen::Vector3f(1.0f, 1.0f, 1.0f),
							Eigen::Vector3f(1.0f,1.0f,1.0f)));
					return;
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
	if (action != GLFW_RELEASE) {
		float movementSpeed = .1;

		if (mods == GLFW_MOD_SHIFT)
			movementSpeed *= 5;

		switch (key) {
			case GLFW_KEY_LEFT:
				// Move to the left
				this->scene->navigate3D = false;
				this->MoveCamera(movementSpeed, 0.);
				return;
			case GLFW_KEY_RIGHT:
				// Move to the right
				this->scene->navigate3D = false;
				this->MoveCamera(-movementSpeed, 0.);
				return;
			case GLFW_KEY_UP:
				// Move to the top
				this->scene->navigate3D = false;
				this->MoveCamera(0., movementSpeed);
				return;
			case GLFW_KEY_DOWN:
				// Move to the bottom
				this->scene->navigate3D = false;
				this->MoveCamera(0., -movementSpeed);
				return;
			case GLFW_KEY_W:
				// Move forward
				this->scene->navigate3D = true;
				this->scene->GetCamera()->camera3DCoordinates
						+= movementSpeed
								* this->scene->GetCamera()->cameraFront;
				return;
			case GLFW_KEY_S:
				// Move backward
				this->scene->navigate3D = true;
				this->scene->GetCamera()->camera3DCoordinates
						-= movementSpeed
								* this->scene->GetCamera()->cameraFront;
				return;
			case GLFW_KEY_A:
				// Move left
				this->scene->navigate3D = true;
				this->scene->GetCamera()->camera3DCoordinates -=
						this->scene->GetCamera()->cameraFront
								.cross(this->scene->GetCamera()->up)
								.normalized() * movementSpeed;
				return;
			case GLFW_KEY_D:
				// Move right
				this->scene->navigate3D = true;
				this->scene->GetCamera()->camera3DCoordinates +=
						this->scene->GetCamera()->cameraFront
								.cross(this->scene->GetCamera()->up)
								.normalized() * movementSpeed;
				return;
			case GLFW_KEY_O:
				// Zoom out
				this->scene->navigate3D = false;
				this->ZoomCamera(movementSpeed);
				return;
			case GLFW_KEY_P:
				// Zoom in
				this->scene->navigate3D = false;
				this->ZoomCamera(-movementSpeed);
				return;
		}
	}
}

void Context::ProcessMouseMovement(double x, double y) {
	if (mouseLeftPressed){
		this->scene->navigate3D = true;
		float xpos = static_cast<float>(x);
		float ypos = static_cast<float>(y);

		if (firstMouse) {
			this->scene->GetCamera()->SetLastX(xpos);
			this->scene->GetCamera()->SetLastY(ypos);
			firstMouse = false;
		}

		float xoffset = (xpos - this->scene->GetCamera()->GetLastX()) * MOUSE_SPEED;
		float yoffset = (this->scene->GetCamera()->GetLastY() - ypos) * MOUSE_SPEED;
		this->scene->GetCamera()->SetLastX(xpos);
		this->scene->GetCamera()->SetLastY(ypos);

		this->scene->GetCamera()->MoveYaw(xoffset);
		this->scene->GetCamera()->MovePitch(yoffset);

		Eigen::Vector3f front;
		front[0] = cos(this->scene->GetCamera()->GetYaw() * EIGEN_PI / PI_DEGREE)
               * cos(this->scene->GetCamera()->GetPitch()* EIGEN_PI / PI_DEGREE);
		front[1] = sin(this->scene->GetCamera()->GetPitch() * EIGEN_PI / PI_DEGREE);
		front[2] = sin(this->scene->GetCamera()->GetYaw() * EIGEN_PI / PI_DEGREE)
               * cos(this->scene->GetCamera()->GetPitch() * EIGEN_PI / PI_DEGREE);

		this->scene->GetCamera()->cameraFront = front.normalized();
	}
}

void Context::ProcessMouseButton(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		if (action == GLFW_PRESS) {
			mouseLeftPressed = true;
		} else if (action == GLFW_RELEASE) {
			mouseLeftPressed = false;
			firstMouse = true;
		}
	}
}

void Context::ProcessMouseScroll(double x, double y) {
	//this->scene->navigate3D = true;
	this->ZoomCamera(-y);
}

int Context::GetNewModuleID() {
	return this->nextModuleID++;
}

void Context::AskForUpdate() {
	this->needToUpdate = true;
}

void Context::AddModule(GUIModule* module) {
	if (module != nullptr)
		this->modules.push_back(module);
}

void Context::SetMesh(Mesh* mesh) {
	if (this->scene == nullptr) {
		this->scene = new Scene();
		if (this->viewer == nullptr)
			this->viewer = new ViewerModule(this);
		this->viewer->GetRenderer()->SetScene(this->scene);
	}
	this->scene->SetMesh(mesh);
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
	return this->windowTitleForced.empty()
			? this->windowTitle
			: this->windowTitleForced;
}

void Context::SetWindowSize(int width, int height) {
	glfwSetWindowSize(this->window, width, height);
	this->windowWidth = width;
	this->windowHeight = height;
}

int Context::GetWindowWidth() {
	return this->windowWidth;
}

int Context::GetWindowHeight() {
	return this->windowHeight;
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

void Context::SetBenchmarkMode(bool benchmark) {
	this->benchmarkMode = benchmark;
}

bool Context::GetBenchmarkMode() {
	return this->benchmarkMode;
}

void Context::SetDebugMode(bool debug) {
	this->debugMode = debug;
}

bool Context::GetDebugMode() {
	return this->debugMode;
}

void Context::SetDarkMode(bool darkMode) {
	if (darkMode) {
		ImGui::StyleColorsDark();
		windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);
		this->viewer->GetRenderer()
				->SetClearColor(Eigen::Vector4f(.1f, .1f, .1f, 1.f));
	} else {
		ImGui::StyleColorsLight();
		windowClearColor = ImVec4(.95f, .95f, .95f, 1.f);
		this->viewer->GetRenderer()
				->SetClearColor(Eigen::Vector4f(1.f, 1.f, 1.f, 1.f));
	}
	this->darkMode = darkMode;
}

bool Context::GetDarkMode() {
	return this->darkMode;
}

CLILoader Context::GetCLI() {
	return this->cli;
}

TOMLLoader Context::GetTOML() {
	return this->toml;
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
							dynamic_cast<SimpleRenderer*>(renderer))) {
						this->SwitchRenderer<SimpleRenderer>();
					} else if (ImGui::MenuItem("Forward shading", "",
							dynamic_cast<ForwardRenderer*>(renderer))) {
						this->SwitchRenderer<ForwardRenderer>();
					// } else if (ImGui::MenuItem("Deferred shading", "",
					// 		dynamic_cast<DeferredRenderer*>(renderer))) {
					// 	this->SwitchRenderer<DeferredRenderer>();
					// } else if (ImGui::MenuItem("Clustured deferred shading", "",
					// 		dynamic_cast<ClusturedRenderer*>(renderer))) {
					// 	this->SwitchRenderer<ClusturedRenderer>();
					}
					ImGui::Separator();
					bool renderingPerMaterial =
							renderer->IsRenderingPerMaterial();
					if (ImGui::MenuItem("One-pass shading", "",
							(!renderingPerMaterial))) {
						renderer->SetRenderingPerMaterial(false);
						if (this->shadersContent != nullptr) {
							this->shadersContent->SetShaders(
									renderer->GetNbShaders(),
									renderer->GetShaders());
						}
					} else if (ImGui::MenuItem("Per-material shading", "",
							renderingPerMaterial)) {
						renderer->SetRenderingPerMaterial(true);
						if (this->shadersContent != nullptr) {
							this->shadersContent->SetShaders(
									renderer->GetNbShaders(),
									renderer->GetShaders());
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Facet culling")) {
					int facetCullingIsEnabled = (int) glIsEnabled(GL_CULL_FACE);
					if (ImGui::MenuItem("Enable facet culling", "",
							facetCullingIsEnabled)) {
						if (facetCullingIsEnabled == (int) GL_TRUE)
							glDisable(GL_CULL_FACE);
						else
							glEnable(GL_CULL_FACE);
					}
					ImGui::Separator();
					int faceCullingMode;
					glGetIntegerv(GL_CULL_FACE_MODE, &faceCullingMode);
					if (ImGui::MenuItem("Front and back", "",
							(faceCullingMode == GL_FRONT_AND_BACK)))
						glCullFace(GL_FRONT_AND_BACK);
					if (ImGui::MenuItem("Front only", "",
							(faceCullingMode == GL_FRONT)))
						glCullFace(GL_FRONT);
					if (ImGui::MenuItem("Back only", "",
							(faceCullingMode == GL_BACK)))
						glCullFace(GL_BACK);
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
				if (ImGui::MenuItem("Show shaders content", "",
						(this->shadersContent != nullptr)))
					this->ToggleShadersContentModule();
				if (ImGui::MenuItem("Show FPS", "",
						(this->imguiFPS != nullptr)))
					this->ToggleImGuiFPSModule();
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
