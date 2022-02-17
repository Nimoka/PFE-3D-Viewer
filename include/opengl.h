#ifndef DEF_OPENGL_H
#define DEF_OPENGL_H

#ifdef __APPLE__
#include <glbinding/gl41core/gl.h>
using namespace gl41core;
#else
#ifdef _WIN32
#include <glbinding/gl32core/gl.h>
using namespace gl32core;
#else
#include <glbinding/gl43core/gl.h>
using namespace gl43core;
#endif
#endif

#include <glbinding/Binding.h>
#include <glbinding-aux/debug.h>

#include <GLFW/glfw3.h>

#endif // DEF_OPENGL_H
