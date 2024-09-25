#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stoi;
using std::ifstream;
using std::stringstream;


#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


#include <GLFW/glfw3.h>
const GLuint ShaderError = static_cast<GLuint>(-1);

// Functions
GLuint setupShaders(GLuint *vertexShader, GLuint *fragmentShader,
                    GLuint *computeShader);
void cleanUpShaders(GLuint *vertexShader, GLuint *fragmentShader,
             GLuint *computeShader, GLuint *shaderProgram);
// Particles struct using cl_vectors and ints

typedef struct {
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint computeShader;
  GLuint shaderProgram;
} Shaders;

#endif