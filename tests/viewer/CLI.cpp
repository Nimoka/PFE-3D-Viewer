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
}


int main(int argc, char** argv) {
	return Catch::Session().run(argc, argv);
}