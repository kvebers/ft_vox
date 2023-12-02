#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::cerr;
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
const GLuint ShaderError = static_cast<GLuint>(-1);

GLuint setupShaders(GLuint *vertexShader, GLuint *fragmentShader,
                    GLuint *computeShader);

#endif