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


static void TestHelp() {
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--help" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	freeArgv(argc, argv);
}

static void TestMissingInput() {
	int argc = 1;
	char **argv = getArgv(argc, std::vector<std::string>{ "command" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	freeArgv(argc, argv);
}
static void TestWrongInput() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-i", TEST_DATA_DIR "configs/default.toml" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestRightInput() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-i", TEST_DATA_DIR "models/cube.ply" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetInputFile() == TEST_DATA_DIR "models/cube.ply");
	freeArgv(argc, argv);
}

static void TestWrongConfig() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-c", TEST_DATA_DIR "configs/defaulttt.toml" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestRightConfig() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-c", TEST_DATA_DIR "configs/default-2.toml" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetConfigFile() == (TEST_DATA_DIR "configs/default-2.toml"));
	freeArgv(argc, argv);
}

static void TestNonIntegerWidth() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--width", "test" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestNegativeWidth() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--width", "-480" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestRightWidth() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--width", "1000" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowWidth() == 1000);
	freeArgv(argc, argv);
}

static void TestNonIntegerHeight() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--height", "test" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestNegativeHeight() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--height", "-480" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestRightHeight() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--height", "1000" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowHeight() == 1000);
	freeArgv(argc, argv);
}

static void TestTitle() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-t", "Test title" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowTitle() == "Test title");
	freeArgv(argc, argv);
}

static void TestExclusiveBenchmark() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-b", "--nb" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestBenchmark() {
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-b" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetBenchmarkMode());
	freeArgv(argc, argv);
}
static void TestNoBenchmark() {
	context->SetBenchmarkMode(true);
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--nb" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(!context->GetBenchmarkMode());
	freeArgv(argc, argv);
}

static void TestExclusiveDebug() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-d", "--nd" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestDebug() {
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "-d" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetDebugMode());
	freeArgv(argc, argv);
}
static void TestNoDebug() {
	context->SetDebugMode(true);
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--nd" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(!context->GetDebugMode());
	freeArgv(argc, argv);
}

static void TestExclusiveTheme() {
	int argc = 3;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--dark", "--light" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);
	freeArgv(argc, argv);
}
static void TestDark() {
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--dark" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetDarkMode());
	freeArgv(argc, argv);
}
static void TestLight() {
	context->SetDarkMode(true);
	int argc = 2;
	char **argv = getArgv(argc, std::vector<std::string>{ "command", "--light" });
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(!context->GetDarkMode());
	freeArgv(argc, argv);
}


TEST_CASE("CLI testing") {
	if (!InitializeGLFW())
		return ERROR_GLFW_INIT;

	context = new Context("");
	context->Init();

	GLFWwindow* window = context->GetWindow();

	SECTION("--help") {
		TestHelp();
	}
	SECTION("-i") {
		TestMissingInput();
		TestWrongInput();
		TestRightInput();
	}
	SECTION("-c") {
		TestWrongConfig();
		TestRightConfig();
	}
	SECTION("--width") {
		TestNonIntegerWidth();
		TestNegativeWidth();
		TestRightWidth();
	}
	SECTION("--height") {
		TestNonIntegerHeight();
		TestNegativeHeight();
		TestRightHeight();
	}
	SECTION("-t") {
		TestTitle();
	}
	SECTION("-b, --nb") {
		TestExclusiveBenchmark();
		TestBenchmark();
		TestNoBenchmark();
	}
	SECTION("-d, --nd") {
		TestExclusiveDebug();
		TestDebug();
		TestNoDebug();
	}
	SECTION("--dark, --light") {
		TestExclusiveTheme();
		TestDark();
		TestLight();
	}

	CleanupEverything();
}
