#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GLFWwindow *window;

int windowSize[2] = { 1280, 800 };
ImVec4 windowClearColor = ImVec4(.2f, .2f, .2f, 1.f);

static void PrintGLFWError(int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int argc, char** argv) {
	/* Initialize GLFW */
	glfwSetErrorCallback(PrintGLFWError);
	if (!glfwInit())
		return 1;

#if defined(__APPLE__)
	const char* glslVersion = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	const char* glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

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

	/* Setup Dear ImGui style */
#ifdef DARK_MODE
	ImGui::StyleColorsDark();
#else
	ImGui::StyleColorsLight();
#endif

	/* Setup Platform/Renderer backends */
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	/* Main loop */
	while (!glfwWindowShouldClose(window)) {
		/* Poll latest events */
		glfwPollEvents();

		/* Start new Dear ImGui frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// TODO: Implement interface

		/* Rendering new frame */
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

	/* Cleanup */
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
