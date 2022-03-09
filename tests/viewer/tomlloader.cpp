#include "opengl.h"

#include <cmath>
#include <ctype.h>
#include <iostream>

#include <backends/imgui_impl_glfw.h>

#define CATCH_CONFIG_MAIN
#define ERROR_GLFW_INIT

#include <catch2/catch.hpp>

#include "context.h"

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

char* CstrFromString(std::string str) {
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}

char** getArgv(int argc, std::vector<std::string> strs) {
	char **argv = (char**) malloc(sizeof(char*) * argc);
	for (int i = 0; i < argc; i++)
		argv[i] = CstrFromString(strs[i]);
	return argv;
}

void freeArgv(int argc, char **argv) {
	for (int i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);
}


static void TestMissingTOML() {
    REQUIRE(!context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test.toml"));
}
static void TestRightTOML() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_existence.toml"));
}

static void TestMissingTitle() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_missing_title.toml"));
}
static void TestWrongTitle() {
    REQUIRE(!context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_bad_title.toml"));
}
static void TestRightTitle() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/default-2.toml"));
    REQUIRE(context->GetWindowTitle() == "3D Viewer");
}

static void TestMissingWidth() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_missing_width.toml"));
}
static void TestWrongWidth() {
    REQUIRE(!context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_bad_width.toml"));
}
static void TestRightWidth() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/default-2.toml"));
    REQUIRE(context->GetWindowWidth() == 640);
}

static void TestMissingHeight() {
    REQUIRE(context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_missing_height.toml"));
}
static void TestWrongHeight() {
    REQUIRE(!context->GetTOML().LoadContext(context,
            TEST_DATA_DIR "configs/test_bad_height.toml"));
}
static void TestRightHeight() {
    REQUIRE(context->GetTOML().LoadContext(context, 
            TEST_DATA_DIR "configs/default-2.toml"));
    REQUIRE(context->GetWindowHeight() == 480);
}


TEST_CASE("TOML testing") {
	if (!InitializeGLFW())
		return ERROR_GLFW_INIT;

	context = new Context("");
	context->Init();

	GLFWwindow* window = context->GetWindow();

	SECTION("TOML file existance") {
		TestMissingTOML();
        TestRightTOML();
	}
	SECTION("Window category") {
        SECTION("title") {
            TestMissingTitle();
            TestWrongTitle();
            TestRightTitle();
        }
        SECTION("width") {
            TestMissingWidth();
            TestWrongWidth();
            TestRightWidth();
        }
        SECTION("height") {
            TestMissingHeight();
            TestWrongHeight();
            TestRightHeight();
        }
	}

	CleanupEverything();
}
