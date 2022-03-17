# PFE 3D Viewer

This project was made as an end-of-study project of Master in Computer Science at the _University of Bordeaux_, on behalf of the _Manao team_ of the _Inria_. It was originally developped in February and March 2022.

It contains three programs:
- a **mesh simplifier**, to reduce the number of faces of a mesh using different parameters,
- a **mesh viewer** called `3DViewer`, to display a mesh in a simple 3D scene,
- and **benchmark synthesis**, a simple Python script that launch the simplifier then benchmarks using the viewer with different parameters, and export a synthesis of the results.

---

## Table of contents

- [PFE 3D Viewer](#pfe-3d-viewer)
	- [Table of contents](#table-of-contents)
	- [Dependencies](#dependencies)
	- [Usage](#usage)
		- [Install dependencies](#install-dependencies)
		- [Compilation](#compilation)
		- [Tests](#tests)
	- [Troubleshooting](#troubleshooting)
		- [Clang-format](#clang-format)

---

## Dependencies

This is a list of dependencies used in this project that you can install by yourself. Note that some of them are required, other can be compiled at the same time as the rest of the project.

- `cmake` _(required)_
- [`glbinding` (`v3.1.0`)](https://github.com/cginternals/glbinding/releases/tag/v3.1.0)

## Usage

### Install dependencies

- **On macOS:**
	- If needed, install `brew` _([steps](https://docs.brew.sh/Installation))_
	- Run the command:
		```
		brew install cmake glbinding
		```

### Compilation

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./compile.sh
		```

### Tests

- **On Linux/macOS:**
	- Run the command at the root of the directory:
		```
		./test.sh
		```

## Troubleshooting

- **When I launch the viewer program, I got an _‘GLFW Error’_ message in the console.**
	- The GLFW version available on your computer may not be compatible with the one specified in the code for your OS: **specifying a previous version of GLFW to use** may do the trick. You can easily change it in the file `src/viewer/main.cpp` in `InitializeGLFW()`, then re-compile and relaunch the program.
- **I opened a large mesh on my ~~outdated~~ computer with no graphics card or an Intel Graphics, but the mesh doesn’t show anything.**
	- Your computer may be too old or underpowered to show the mesh. **Buy a new one.**


### Clang-format

- **Launch the clang-format on .cpp, .hpp, .c, .h files**
  - set the path of .clang-format in vscode : go to Extension, install clang-format by xaver, at Manage-> extension Setting -> Clang-format:Style -> input the full path of .clang-format.
  - find . -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {} \ ; .  
