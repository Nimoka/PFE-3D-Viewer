#include "cliloader.h"

#include "context.h"

int CLILoader::LoadContext(void *c, int argc, char **argv) {
	Context *context = (Context*) c;
	/* Create CLI context */

	CLI::App app{DEFAULT_WINDOW_TITLE};

	std::string windowTitle;
	int windowWidth = DEFAULT_WINDOW_WIDTH;
	int windowHeight = DEFAULT_WINDOW_HEIGHT;
	int pLight = DEFAULT_NB_POINT_LIGHT;

	std::string inputFile, configFile;
	bool benchmarkMode = false, noBenchmarkMode = false, debugMode = false,
			noDebugMode = false, darkMode = false, lightMode = false,
			simpleShadingMode = false, forwardShadingMode = false,
			deferredShadingMode = false, forceUnsortedMeshMode = false;

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
	app.add_option("--pl",pLight,"Configure the number of point light in the scene")
			->check(CLI::PositiveNumber);

	CLI::Option *simpleShading = app.add_flag("--ss, --simple",
			simpleShadingMode,
			"Run the program with simple shading");
	CLI::Option *forwardShading = app.add_flag("--fs, --forward",
			forwardShadingMode,
			"Run the program with forward shading");
	// CLI::Option *deferredShading = app.add_flag("--ds, --deferred",
	// 		deferredShadingMode,
	// 		"Run the program with deferred shading");
	simpleShading->excludes(forwardShading);
	// simpleShading->excludes(deferredShading);
	forwardShading->excludes(simpleShading);
	// forwardShading->excludes(deferredShading);
	// deferredShading->excludes(simpleShading);
	// deferredShading->excludes(forwardShading);

	CLI::Option *forceUnsortedMesh = app.add_flag("--fu, --force-unsorted",
			forceUnsortedMeshMode,
			"Force the program to don’t sort the input mesh if it needs to");

	CLI::Option *benchmark = app.add_flag("-b, --benchmark",
			benchmarkMode,
			"Run the program in benchmark mode");
	CLI::Option *noBenchmark = app.add_flag("--nb, --no-benchmark",
			noBenchmarkMode,
			"Run the program normally, without benchmark mode");
	benchmark->excludes(noBenchmark);
	noBenchmark->excludes(benchmark);

	CLI::Option *debug = app.add_flag("-d, --debug",
			debugMode,
			"Enable debug mode (tools in menu bar)");
	CLI::Option *noDebug = app.add_flag("--nd, --no-debug",
			noDebugMode,
			"Disable debug mode (tools in menu bar)");
	debug->excludes(noDebug);
	noDebug->excludes(debug);

	CLI::Option *dark = app.add_flag("--dark",
			darkMode,
			"Enable dark mode");
	CLI::Option *light = app.add_flag("--light",
			lightMode,
			"Enable light mode");
	dark->excludes(light);
	light->excludes(dark);

	/* Parse CLI options */

	CLI11_PARSE(app, argc, argv);

	/* Process CLI options found (if needed) */

	// PLY file to load
	if (!inputFile.empty())
		context->SetInputFile(inputFile);

	// Configuration file
	if (!configFile.empty()) {
		if (!context->GetTOML().LoadContext(context, configFile))
			return ERROR_CLI_MISS_TOML;
		context->SetConfigFile(configFile);
	}

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

	// Set point light number
	if(pLight>1)
		context->SetPointLightNumber(pLight);

	// Set simple shading
	if (simpleShadingMode)
		context->SetSimpleShading();

	// Set forward shading
	if (forwardShadingMode)
		context->SetForwardShading();

	// Set deferred shading
	// if (deferredShadingMode)
	// 	context->SetDeferredShading();

	// Force unsorted mesh
	if (forceUnsortedMeshMode)
		context->SetForceUnsortedMesh(forceUnsortedMeshMode);

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
