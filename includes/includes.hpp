#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <exception>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <GLFW/glfw3.h>

#endif