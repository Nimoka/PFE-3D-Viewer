cmake_policy(SET CMP0048 NEW)

project(PFE3DViewer)

cmake_minimum_required(VERSION 3.2.0)
include(CheckCXXCompilerFlag)

# CMake configuration ==========================================

set(BUILD_LIBS ON)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})
set(BUILD_DIR ${ROOT_DIR}/build)

if (APPLE)
	set(CMAKE_MACOSX_RPATH ON)
	set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)
	set(CMAKE_PREFIX_PATH /usr/local/opt/)
	set(GL_SILENCE_DEPRECATION ON)
	add_definitions(-DGL_SILENCE_DEPRECATION)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

	if (HAS_CPP11_FLAG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
		message(FATAL_ERROR "Unsupported compiler -- requires C++11 support!")
	endif()
	if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
	endif()
endif ()

if (DEBUG_ALL)
	add_definitions(-DDEBUG_OPENGL_CONTEXT)
endif ()

# Libraries ====================================================

set(EXTERNAL external)

# GLFW ---------------------------------------------------------

set(GLFW_BUILD_EXAMPLES		OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS		OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS			OFF CACHE BOOL "")
set(GLFW_INSTALL			OFF CACHE BOOL "")

set(GLFW_DIR ${ROOT_DIR}/${EXTERNAL}/glfw)
set(GLFW_INCLUDE ${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_INCLUDE})

# Dear ImGui ---------------------------------------------------

set(IMGUI_DIR ${ROOT_DIR}/${EXTERNAL}/imgui)
set(IMGUI_INCLUDE ${IMGUI_DIR})
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE})
include_directories(${IMGUI_INCLUDE})

# miniply ------------------------------------------------------

set(MINIPLY_DIR ${ROOT_DIR}/${EXTERNAL}/miniply)
add_library(miniply ${MINIPLY_DIR}/miniply.cpp)
target_include_directories(miniply PUBLIC ${MINIPLY_DIR})

# TOML11 -------------------------------------------------------

set(TOML11_DIR ${ROOT_DIR}/${EXTERNAL}/toml11)
include_directories(${TOML11_DIR})

# CLI11 --------------------------------------------------------

set(CLI11_DIR ${ROOT_DIR}/${EXTERNAL}/CLI11)
add_subdirectory(${CLI11_DIR})

# FileBrowser --------------------------------------------------

set(FILEBROWSER_DIR ${ROOT_DIR}/${EXTERNAL}/FileBrowser)
add_library(FileBrowser ${FILEBROWSER_DIR}/ImGuiFileBrowser.cpp)
include_directories(${FILEBROWSER_DIR})

# Eigen --------------------------------------------------------

set(EIGEN_DIR ${ROOT_DIR}/${EXTERNAL}/eigen)
include_directories(${EIGEN_DIR})

# glbinding ----------------------------------------------------

set(BUILD_SHARED_LIBS		OFF CACHE BOOL "")
set(OPTION_BUILD_TOOLS		OFF CACHE BOOL "")
set(OPTION_BUILD_EXAMPLES	OFF CACHE BOOL "")

if (BUILD_LIBS)
	set(GLBINDING_DIR ${ROOT_DIR}/${EXTERNAL}/glbinding)
	set(GLBINDING_INCLUDE
			${GLBINDING_DIR}/source/glbinding/include
			${GLBINDING_DIR}/source/glbinding-aux/include
			${GLBINDING_DIR}/3rdparty/KHR/include)
	add_subdirectory(${GLBINDING_DIR})
	include_directories(${GLBINDING_INCLUDE})
else ()
	find_package(glbinding REQUIRED)
endif ()

## globjects ----------------------------------------------------
#
#if (BUILD_LIBS)
#	set(GLOBJECTS_DIR ${ROOT_DIR}/${EXTERNAL}/globjects)
#	set(GLOBJECTS_INCLUDE ${GLOBJECTS_DIR}/source/globjects/include)
#	add_subdirectory(${GLOBJECTS_DIR})
#	include_directories(${GLOBJECTS_INCLUDE})
#else ()
#	find_package(globjects REQUIRED)
#endif ()

# OpenGL -------------------------------------------------------

find_package(OpenGL REQUIRED)

# Includes =====================================================

include_directories(include)

# Executables ==================================================

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")
add_definitions(-DTEST_DATA_DIR="${PROJECT_SOURCE_DIR}/tests/data/")

# Viewer -------------------------------------------------------

set(VIEWER_NAME 3DViewer)
file(GLOB VIEWER_SOURCES
		${ROOT_DIR}/src/viewer/*.cpp
		${ROOT_DIR}/src/viewer/modules/*.cpp
		${ROOT_DIR}/src/viewer/renderers/*.cpp
		${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
file(GLOB VIEWER_HEADERS
		${ROOT_DIR}/include/viewer/*.h
		${ROOT_DIR}/include/viewer/modules/*.h
		${ROOT_DIR}/include/viewer/renderers/*.h
		${IMGUI_DIR}/backends/imgui_impl_glfw.h
		${IMGUI_DIR}/backends/imgui_impl_opengl3.h
		${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h)

set(VIEWER_LIBRARIES
		glfw
		${GLFW_LIBRARIES}
		glbinding::glbinding
		glbinding::glbinding-aux
		imgui
		miniply
		FileBrowser
		CLI11
		${OPENGL_LIBRARIES})

set(VIEWER_INCLUDE
		${ROOT_DIR}/include/viewer/
		${GLFW_INCLUDE}
		${IMGUI_INCLUDE}
		${IMGUI_DIR}/backends/
		${MINIPLY_DIR}
		${TOML11_DIR}
		${CLI11_DIR}
		${EIGEN_DIR})
if (BUILD_LIBS)
set(VIEWER_INCLUDE
			${VIEWER_INCLUDE}
			${GLBINDING_INCLUDE})
endif ()

add_executable(${VIEWER_NAME}
		${VIEWER_SOURCES}
		${VIEWER_HEADERS})
target_include_directories(${VIEWER_NAME} PUBLIC ${VIEWER_INCLUDE})
target_link_libraries(${VIEWER_NAME} PUBLIC ${VIEWER_LIBRARIES})
target_compile_definitions(${VIEWER_NAME} PRIVATE GLFW_INCLUDE_NONE)

# Tests ========================================================

if (BUILD_TESTING)
	# Libraries ================================================

	# Catch2 ---------------------------------------------------

	set(CATCH2_DIR ${EXTERNAL}/Catch2)
	add_subdirectory(${CATCH2_DIR})
	include_directories(${CATCH2_DIR}/src/catch2)

	# Executables ==============================================

	set(TESTS tests)
	add_subdirectory(${TESTS})
endif ()
