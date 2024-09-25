#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stoi;

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

// Functions
GLuint setupShaders(GLuint *vertexShader, GLuint *fragmentShader,
                    GLuint *computeShader);
void cleanUp(GLuint *vertexShader, GLuint *fragmentShader,
             GLuint *computeShader, GLuint *shaderProgram);
// Particles struct using cl_vectors and ints
typedef struct {
  cl_float3 position;
  cl_float3 velocity;
  cl_int mass;
  cl_int4 color;
  cl_int lifetime;
  cl_float3 acceleration;
  cl_float radius;
} Particle;


typedef struct {
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint computeShader;
  GLuint shaderProgram;
} Shaders;


typedef struct {
  cl_context context;
  cl_command_queue queue;
  cl_platform_id platform;
  cl_device_id device;
} OpenCompute;


#endif