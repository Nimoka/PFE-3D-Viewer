cmake_policy(SET CMP0048 NEW)

project(PFE3DViewer)

cmake_minimum_required(VERSION 3.2.0)
include(CheckCXXCompilerFlag)

# CMake configuration ==========================================

if (APPLE)
	set(CMAKE_MACOSX_RPATH ON)
	set(GL_SILENCE_DEPRECATION ON)
	add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

	if (HAS_CPP14_FLAG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	elseif (HAS_CPP11_FLAG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
		message(FATAL_ERROR "Unsupported compiler -- requires C++11 support!")
	endif()
	if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
	endif()
endif()

# Libraries ====================================================

set(EXTERNAL external)

# GLFW ---------------------------------------------------------

set(GLFW_DIR ${EXTERNAL}/glfw)
set(GLFW_INCLUDE ${GLFW_DIR}/include)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_INCLUDE})

# Dear ImGui ---------------------------------------------------

set(IMGUI_DIR ${EXTERNAL}/imgui)
add_library(imgui ${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
include_directories(${IMGUI_DIR})

# miniply ------------------------------------------------------

set(MINIPLY_DIR ${EXTERNAL}/miniply)
add_library(miniply ${MINIPLY_DIR}/miniply.cpp)
target_include_directories(miniply PUBLIC ${MINIPLY_DIR})

# Includes =====================================================

include_directories(include)

# Executables ==================================================

# Viewer -------------------------------------------------------

set(VIEWER_NAME 3DViewer)
file(GLOB VIEWER_SOURCES
		src/viewer.cpp
		src/plyreader.cpp
		src/utils.cpp
		src/guimodules/*.cpp
		${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
file(GLOB VIEWER_HEADERS
		include/mesh.h
		include/plyreader.h
		include/utils.h
		include/guimodules/*.h
		${IMGUI_DIR}/backends/imgui_impl_glfw.h
		${IMGUI_DIR}/backends/imgui_impl_opengl3.h
		${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h)
add_executable(${VIEWER_NAME}
		${VIEWER_SOURCES}
		${VIEWER_HEADERS})
target_link_libraries(${VIEWER_NAME} PUBLIC glfw ${GLFW_LIBRARIES} imgui miniply)
target_include_directories(${VIEWER_NAME} PUBLIC ${GLFW_INCLUDE} ${IMGUI_INCLUDE} ${IMGUI_DIR}/backends/ ${MINIPLY_DIR})

find_package(OpenGL REQUIRED)
target_link_libraries(${VIEWER_NAME} PUBLIC ${OPENGL_LIBRARIES})
