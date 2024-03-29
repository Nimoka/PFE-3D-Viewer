#include "opengl.h"

#include <cmath>
#include <ctype.h>
#include <iostream>

#include <backends/imgui_impl_glfw.h>
#include <time.h>
#include "context.h"

#define ENABLE_HIGH_DPI

#define ERROR_GLFW_INIT		1

Context* context;
std::string glslVersion;

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
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (GLint) GL_TRUE);

#ifdef ENABLE_HIGH_DPI
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
#else
#ifdef _WIN32
		glslVersion = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (GLint) GL_TRUE);
#else
		glslVersion = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (GLint) GL_TRUE);
#endif
#endif
	}

	return err;
}

void CleanupGLFW() {
	glfwTerminate();
}

void CleanupEverything() {
	delete context;
	CleanupGLFW();
}

void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	context->ProcessKeyboardInput(key, scancode, action, mods);
}

void ProcessMouseMovement(GLFWwindow* window, double x, double y) {
	ImGui_ImplGlfw_CursorPosCallback(window, x, y);
	context->ProcessMouseMovement(x, y);
}

void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods) {
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	context->ProcessMouseButton(button, action, mods);
}

void ProcessMouseScroll(GLFWwindow* window, double x, double y) {
	ImGui_ImplGlfw_ScrollCallback(window, x, y);
	context->ProcessMouseScroll(x, y);
}

int main(int argc, char** argv) {
	srand(time(0));
	if (!InitializeGLFW())
		return ERROR_GLFW_INIT;

	context = new Context(glslVersion);
	context->Init();

	/* Load context from CLI and configuration files */

	{
		int error = context->LoadOptions(argc, argv);
		if (error) {
			CleanupEverything();
			return error;
		}
	}

	if (context->GetBenchmarkMode()) {
		context->LaunchBenchmark();
	} else {
		/* Set GLFW callback functions */

		GLFWwindow* window = context->GetWindow();
		glfwSetKeyCallback(window, ProcessKeyboardInput);
		glfwSetCursorPosCallback(window, ProcessMouseMovement);
		glfwSetMouseButtonCallback(window, ProcessMouseButton);
		glfwSetScrollCallback(window, ProcessMouseScroll);

		/* Launch the main loop */

		context->Launch();
	}

	CleanupEverything();

	return 0;
}
