#include "opengl.h"

#include <cmath>
#include <iostream>
#include <ctype.h>

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

void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	context->ProcessKeyboardInput(key, scancode, action, mods);
}

void ProcessMouseMovement(GLFWwindow* window, double x, double y) {
	context->ProcessMouseMovement(x, y);
}

void ProcessMouseButton(GLFWwindow* window, int button, int action, int mods) {
	context->ProcessMouseButton(button, action, mods);
}

void ProcessMouseScroll(GLFWwindow* window, double x, double y) {
	context->ProcessMouseScroll(x, y);
}

int main(int argc, char** argv) {
	if (!InitializeGLFW())
		return ERROR_GLFW_INIT;

	context = new Context(glslVersion);
	context->Init();

	/* Set GLFW callback functions */

	{
		GLFWwindow* window = context->GetWindow();
		glfwSetKeyCallback(window, ProcessKeyboardInput);
		// glfwSetCursorPosCallback(window, ProcessMouseMovement);
		// glfwSetMouseButtonCallback(window, ProcessMouseButton);
		// glfwSetScrollCallback(window, ProcessMouseScroll);
	}

	/* Load context from CLI and configuration files */

	{
		int error = context->LoadCLIContext(argc, argv);
		if (error) {
			CleanupEverything();
			return error;
		}
	}

	context->Launch();

	CleanupEverything();

	return 0;
}
