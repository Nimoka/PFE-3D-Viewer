#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "plyreader.h"
#include "guimodules/module.h"
#include "guimodules/plycontent.h"

#define ENABLE_DARK_MODE
#define ENABLE_HIGH_DPI

#define ERR_GLFW		1
#define ERR_IMGUI		2

GLFWwindow *window;
ImVec4 windowClearColor;
int windowSize[2] = { 1280, 800 };
float windowScale = 1.;
std::vector<GUIModule*> windowModules;

Mesh* mesh = nullptr;

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
#if _WIN32
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
#ifdef ENABLE_HIGH_DPI
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		float xscale, yscale;
		glfwGetMonitorContentScale(monitor, &xscale, &yscale);
		if (xscale > 1 || yscale > 1) {
			highDPIscaleFactor = xscale;
			glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		}
#endif
#endif
	}

	return err;
}

void CleanupGLFW() {
	glfwTerminate();
}

int InitializeImGui() {
	/* Create main window */
	window = glfwCreateWindow(windowSize[0], windowSize[1], "3D Viewer", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

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

#ifdef ENABLE_HIGH_DPI
	ImGuiStyle &style = ImGui::GetStyle();
	style.ScaleAllSizes(windowScale);
#endif

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

int main(int argc, char** argv) {
	if (!InitializeGLFW())
		return ERR_GLFW;
	if (InitializeImGui())
		return ERR_IMGUI;

	PLYReader* plyReader1 = new PLYReader("../data/models/color_cube.ply");
	if (plyReader1->Load())
		windowModules.push_back(new PlyContentModule("color_cube.ply", plyReader1->GetMesh()));
	PLYReader* plyReader2 = new PLYReader("../data/models/matid_cube.ply");
	if (plyReader2->Load())
		windowModules.push_back(new PlyContentModule("matid_cube.ply", plyReader2->GetMesh()));
	PLYReader* plyReader3 = new PLYReader("../data/models/basic_cube.ply");
	if (plyReader3->Load())
		windowModules.push_back(new PlyContentModule("basic_cube.ply", plyReader3->GetMesh()));
	PLYReader* plyReader4 = new PLYReader("../data/models/gilet_union.ply");
	if (plyReader4->Load())
		windowModules.push_back(new PlyContentModule("gilet_union.ply", plyReader4->GetMesh()));
	PLYReader* plyReader5 = new PLYReader("../data/models/doesntexists.ply");
	if (plyReader5->Load())
		windowModules.push_back(new PlyContentModule("Wait, it exists?", plyReader5->GetMesh()));

	/* Main loop */
	while (!glfwWindowShouldClose(window)) {
		/* Poll latest events */
		glfwPollEvents();

		StartNewImGuiFrame();

		for (auto i: windowModules)
			i->Render();

		RenderImGuiFrame();
	}

	CleanupImGui();
	CleanupGLFW();

	return 0;
}
