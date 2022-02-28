#include <iostream>

#include "catch2/catch_all.hpp"

#include "context.h"

Context* context;

char* CstrFromString(std::string str) {
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}


static void TestHelp() {
	int argc = 2;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("--help");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);

	delete [] argv[0], argv[1];
	free(argv);
}

static void TestMissingInput() {
	int argc = 1;
	char **argv = (char**) malloc(sizeof(char*));
	argv[0] = CstrFromString("command");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0];
	free(argv);
}
static void TestWrongInput() {
	int argc = 3;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "configs/default.toml");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2];
	free(argv);
}
static void TestRightInput() {
	int argc = 3;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetInputFile() == DATA_DIR "models/cube.ply");

	delete [] argv[0], argv[1], argv[2];
	free(argv);
}

static void TestWrongConfig() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("-c");
	argv[4] = CstrFromString(DATA_DIR "configs/defaulttt.toml");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestRightConfig() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("-c");
	argv[4] = CstrFromString(DATA_DIR "configs/default-2.toml");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetConfigFile() == (DATA_DIR "configs/default-2.toml"));

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}

static void TestNonIntegerWidth() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--width");
	argv[4] = CstrFromString("wowza");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestNegativeWidth() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--width");
	argv[4] = CstrFromString("-480");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestRightWidth() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--width");
	argv[4] = CstrFromString("1000");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowWidth() == 1000);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}

static void TestNonIntegerHeight() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--height");
	argv[4] = CstrFromString("wowza");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestNegativeHeight() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--height");
	argv[4] = CstrFromString("-480");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestRightHeight() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--height");
	argv[4] = CstrFromString("1000");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowHeight() == 1000);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}

static void TestTitle() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("-t");
	argv[4] = CstrFromString("This is my test title");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetWindowTitle() == "This is my test title");

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}

static void TestExclusiveBenchmark() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("-b");
	argv[4] = CstrFromString("--nb");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestBenchmark() {
	int argc = 4;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("-b");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetBenchmarkMode());

	delete [] argv[0], argv[1], argv[2], argv[3];
	free(argv);
}
static void TestNoBenchmark() {
	context->SetBenchmarkMode(true);

	int argc = 4;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--nb");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(!context->GetBenchmarkMode());

	delete [] argv[0], argv[1], argv[2], argv[3];
	free(argv);
}

static void TestExclusiveTheme() {
	int argc = 5;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--dark");
	argv[4] = CstrFromString("--light");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) != 0);

	delete [] argv[0], argv[1], argv[2], argv[3], argv[4];
	free(argv);
}
static void TestLight() {
	int argc = 4;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--dark");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(context->GetDarkMode());

	delete [] argv[0], argv[1], argv[2], argv[3];
	free(argv);
}
static void TestDar() {
	context->SetDarkMode(true);

	int argc = 4;
	char **argv = (char**) malloc(sizeof(char*) * argc);
	argv[0] = CstrFromString("command");
	argv[1] = CstrFromString("-i");
	argv[2] = CstrFromString(DATA_DIR "models/cube.ply");
	argv[3] = CstrFromString("--light");
	REQUIRE(context->GetCLI().LoadContext(context, argc, argv) == 0);
	REQUIRE(!context->GetDarkMode());

	delete [] argv[0], argv[1], argv[2], argv[3];
	free(argv);
}


TEST_CASE("CLI testing") {
	context = new Context("");
	context->Init();

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
}


int main(int argc, char** argv) {
	return Catch::Session().run(argc, argv);
}