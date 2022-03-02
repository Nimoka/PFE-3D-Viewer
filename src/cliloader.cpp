#include "cliloader.h"

#include "context.h"

int CLILoader::LoadContext(void *c, int argc, char **argv) {
    Context *context = (Context*) c;
    /* Create CLI context */

	CLI::App app{DEFAULT_WINDOW_TITLE};

	std::string windowTitle;
	int windowWidth = DEFAULT_WINDOW_WIDTH, windowHeight = DEFAULT_WINDOW_HEIGHT;
	std::string inputFile, configFile;
	bool benchmarkMode = false, noBenchmarkMode = false, debugMode = false, noDebugMode = false, darkMode = false, lightMode = false;

	/* Set CLI options */

	app.add_option("-i, --input", inputFile, "PLY file to load")
			->check(CLI::ExistingFile)->check(FileWithExtension("ply"));
	app.add_option("-c, --config", configFile, "Configuration file")
			->check(CLI::ExistingFile)->check(FileWithExtension("toml"));
	app.add_option("--width", windowWidth, "Window’s width (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("--height", windowHeight, "Window’s height (pixels)")
			->check(CLI::PositiveNumber);
	app.add_option("-t, --title", windowTitle, "Window title");

	CLI::Option *benchmark = app.add_flag("-b, --benchmark", benchmarkMode, "Run the program in benchmark mode");
	CLI::Option *noBenchmark = app.add_flag("--nb, --no-benchmark", noBenchmarkMode, "Run the program normally, without benchmark mode");
	benchmark->excludes(noBenchmark);
	noBenchmark->excludes(benchmark);

	CLI::Option *debug = app.add_flag("-d, --debug", debugMode, "Enable debug mode (tools in menu bar)");
	CLI::Option *noDebug = app.add_flag("--nd, --no-debug", noDebugMode, "Disable debug mode (tools in menu bar)");
	debug->excludes(noDebug);
	noDebug->excludes(debug);

	CLI::Option *dark = app.add_flag("--dark", darkMode, "Enable dark mode");
	CLI::Option *light = app.add_flag("--light", lightMode, "Enable light mode");
	dark->excludes(light);
	light->excludes(dark);

	/* Parse CLI options */

	CLI11_PARSE(app, argc, argv);

	/* Process CLI options found (if needed) */

	// PLY file to load
	if (!inputFile.empty())
		context->SetInputFile(inputFile);

	// Configuration file
	if (!configFile.empty())
		if (!context->LoadTOMLContext(configFile))
			return ERROR_CLI_MISS_TOML;

	// Window’s size
	if (windowWidth && windowHeight) 
		context->SetWindowSize(windowWidth, windowHeight);
	else if (windowWidth)			 
		context->SetWindowSize(windowWidth, DEFAULT_WINDOW_HEIGHT);
	else if (windowHeight)			 
		context->SetWindowSize(DEFAULT_WINDOW_WIDTH, windowHeight);

	// Window’s title
	if (!windowTitle.empty())
		context->SetForcedWindowTitle(windowTitle);

	// Benchmark mode
	if (benchmarkMode || noBenchmarkMode)
		context->SetBenchmarkMode(benchmarkMode);

	// Debug mode
	if (debugMode || noDebugMode)
		context->SetDebugMode(debugMode);

	// Window’s style
	if (darkMode || lightMode)
		context->SetDarkMode(darkMode);

	return 0;
}