# PFE 3D Viewer

This project was made as an end-of-study project of Master in Computer Science at the _University of Bordeaux_, on behalf of the _Manao team_ of the _Inria_. It was originally developped in February and March 2022.

The project consists on three programs:
- a **mesh simplifier** called `3DDecimator`, to reduce the number of faces of a mesh using different parameters _(available [here](https://github.com/Nimoka/PFE-3D-Decimator))_,
- a **mesh viewer** called `3DViewer`, to display a mesh in a simple 3D scene,
- and **benchmark synthesis**, a simple Python script that launch the simplifier then benchmarks using the viewer with different parameters, and export a synthesis of the results.

---

## Table of contents

- [Dependencies](#dependencies)
- [Usage](#usage)
	- [Install dependencies](#install-dependencies)
	- [Compilation](#compilation)
	- [Launch the viewer](#launch-the-viewer)
	- [Launch a benchmark](#launch-a-benchmark)
	- [Tests](#tests)
	- [Generate documentation](#generate-documentation)
- [Shortcuts](#shortcuts)
	- [Viewer](#viewer)
- [Troubleshooting](#troubleshooting)
- [Usage of utilitaries](#usage-of-utilitaries)
	- [Clang-format](#clang-format)

---

## Dependencies

This is a list of dependencies used in this project that you can install by yourself. Note that some of them are required, other can be compiled at the same time as the rest of the project.

- `cmake` _(required)_
- [`glbinding` (`v3.1.0`)](https://github.com/cginternals/glbinding/releases/tag/v3.1.0)
- `doxygen` (for documentation generation)

## Usage

### Install dependencies

- **On macOS:**
	- If needed, install `brew` _([steps](https://docs.brew.sh/Installation))_
	- Run the command:
		```
		brew install cmake glbinding doxygen
		```

### Compilation

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./compile.sh
		```

### Launch the viewer

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./build/3DViewer
		```
		- Open a PLY mesh file: `-i <filepath>`
		- Open a TOML configuration file: `-c <filepath>`
		- Launch benchmark mode: `-b`
		- Launch using the _simple shading_ renderer: `--simple`
		- Launch using the _forward shading_ renderer: `--forward`
		- Launch with a number of point lights: `--pl <number>`
		- More arguments are listed with `--help`

### Launch a benchmark

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		python3 benchmark.py
		```
		- Arguments will be send to the viewer app.

### Tests

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./test.sh
		```

### Generate documentation

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./generate_docs.sh
		```
	- The documentation is created in the subfolder `docs/viewer/`.

## Shortcuts

### Viewer

- **Application:**
	- <kbd>Ctrl</kbd>+<kbd>O</kbd>: open a PLY mesh file
	- <kbd>Ctrl</kbd>+<kbd>Q</kbd>: quit the app
	- Key <kbd>Tab</kbd>: show/hide menu and subwindow
- **Renderer:**
	- Key <kbd>R</kbd>: reload the shaders
- **Lights:**
	- Key <kbd>L</kbd>: add a directional light to the scene
	- Key <kbd>Y</kbd>: add a random point light to the covering sphere of the scene
- **Camera:**
	- **Around mode:**
		- Movements: <kbd>Up</kbd>, <kbd>Left</kbd>, <kbd>Down</kbd>, <kbd>Right</kbd>
		- Zoom: <kbd>O</kbd> (zoom out), <kbd>P</kbd> (zoom in)
		- In this mode, the camera will always look to the center of the mesh.
	- **Free mode:**
		- Movements: <kbd>Z</kbd>, <kbd>Q</kbd>, <kbd>S</kbd>, <kbd>D</kbd>
		- You can also drag the mouse over the window to move the camera around.
	- In both modes, <kbd>Shift</kbd> will increase camera speed.

## Troubleshooting

- **When I launch the viewer program, I got an _‘GLFW Error’_ message in the console.**
	- The GLFW version available on your computer may not be compatible with the one specified in the code for your OS: **specifying a previous version of GLFW to use** may do the trick. You can easily change it in the file `src/viewer/main.cpp` in `InitializeGLFW()`, then re-compile and relaunch the program.
- **I opened a large mesh on my ~~outdated~~ computer with no graphics card or an Intel Graphics, but the mesh doesn’t show anything.**
	- Your computer may be too old or underpowered to show the mesh. **Buy a new one.**

## Usage of utilitaries

### Clang-format

- **Launch the clang-format on `.cpp`, `.hpp`, `.c`, `.h` files**
	- Set the path of `.clang-format` in VS Code:
		1. Go to _Extension_
		2. Install `clang-format` by _xaver_
		3. At _Manage_>_Extension Setting_, set _Clang-format:Style_ to the input the full path of `.clang-format`
	- `find . -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {} \ ; .`
