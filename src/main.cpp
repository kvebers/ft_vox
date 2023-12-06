#include "../includes/includes.hpp"
#include <OpenCL/OpenCL.h>
#include <OpenGL/OpenGL.h>
#include <exception>
#include <string>

int errorManagment(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Not enought arguments" << std::endl;
    exit(1);
  }
  int canConvert = 0;
  try {
    canConvert = std::stoi(argv[1]);
  } catch (std::exception &e) {
    std::cerr << "Conversion error " << e.what() << std::endl;
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
}

void mainLoop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

int main(int argc, char **argv) {
  // variables
  GLuint vertexShader, fragmentShader, computeShader;
  // cl_context clContext;
  // cl_command_queue clQueue;
  // cl_program clProgram;

  errorManagment(argc, argv);
  // initialisation of GLFW Window
  GLFWwindow *window =
      glfwCreateWindow(1920, 1080, "Particle System", nullptr, nullptr);
  randomWindowManagment(window);
  // clearing color to black instead of read
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // setup for the vertex, fragment and compute shaders
  GLuint shaderProgram =
      setupShaders(&vertexShader, &fragmentShader, &computeShader);
  if (shaderProgram == ShaderError) {
    glfwTerminate();
    exit(-1);
  }
  glUseProgram(shaderProgram);
  // main loop of the program
  mainLoop(window);
  // cleanup of the program
  cleanUp(&vertexShader, &fragmentShader, &computeShader, &shaderProgram);
  return 0;
}