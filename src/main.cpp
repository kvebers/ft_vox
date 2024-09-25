#include "../includes/includes.hpp"
#include <OpenCL/OpenCL.h>
#include <OpenGL/OpenGL.h>
#include <exception>
#include <string>

int errorManagment(int argc, char **argv) {
  int canConvert = 0;
  if (argc != 2) {
    std::cerr << "Not enought arguments" << std::endl;
    exit(1);
  }
  try {
    canConvert = stoi(argv[1]);
  } catch (std::exception &e) {
    std::cerr << "Conversion error " << e.what() << std::endl;
    exit(1);
  }
  canConvert = stoi(argv[1]);
  if (canConvert < 0) {
    std::cerr << "Invalid argument" << std::endl;
    exit(1);
  }
  if (canConvert > 5000000) {
    std::cerr << "Invalid argument" << std::endl;
    exit(1);
  }
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(1);
  }
  return canConvert;
}

void randomWindowManagment(GLFWwindow *window) {
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(1);
  }
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    glfwTerminate();
    exit(1);
  }
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void mainLoop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main(int argc, char **argv) {
  Shaders shaders;
  OpenCompute openCompute;
  
  int numberOfParticles = errorManagment(argc, argv);
  clGetPlatformIDs(1, &openCompute.platform, nullptr);
  clGetDeviceIDs(openCompute.platform, CL_DEVICE_TYPE_GPU, 1, &openCompute.device, nullptr);
  openCompute.context = clCreateContext(nullptr, 1, &openCompute.device, nullptr, nullptr, nullptr);
  openCompute.queue = clCreateCommandQueue(openCompute.context, openCompute.device, 0, nullptr);
  GLFWwindow *window = glfwCreateWindow(1920, 1080, "Particle System", nullptr, nullptr);
  randomWindowManagment(window);
  shaders.shaderProgram = setupShaders(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader);
  if (shaders.shaderProgram == ShaderError) { glfwTerminate(); exit(-1);}
  glUseProgram(shaders.shaderProgram);
  mainLoop(window);
  cleanUp(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader, &shaders.shaderProgram);
  return 0;
}
