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

string loadCLSource(const string &fileName) {
  ifstream file(fileName);
  if (!file.is_open()) {
    cerr << "Failed to open OpenCL source file: " << fileName << endl;
    exit(1);
  }
  stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void initOpenClProgram(OpenCompute *compute, int numberOfParticles)
{
  string sourceCode = loadCLSource("cl/particles.cl");
  const char *source = sourceCode.c_str();
  size_t sourceSize = sourceCode.size();
  compute->program = clCreateProgramWithSource(compute->context, 1, &source, &sourceSize, nullptr);
  compute->result = clBuildProgram(compute->program, 1, &compute->device, nullptr, nullptr, nullptr);
  if (compute->result != CL_SUCCESS) { cerr << "OpenCL Program Build Error: " << endl; exit(1);}
  compute->kernel = clCreateKernel(compute->program, "particles", nullptr);
  clSetKernelArg(compute->kernel , 0, sizeof(cl_mem), &compute->glParticleBuffer);
  clSetKernelArg(compute->kernel , 1, sizeof(cl_uint), &numberOfParticles);
  size_t iNeedSizeT = numberOfParticles;
  clEnqueueAcquireGLObjects(compute->queue, 1, &compute->glParticleBuffer, 0, nullptr, nullptr);
  clEnqueueNDRangeKernel(compute->queue, compute->kernel, 1, nullptr, &iNeedSizeT, nullptr, 0, nullptr, nullptr);
  clEnqueueReleaseGLObjects(compute->queue, 1, &compute->glParticleBuffer, 0, nullptr, nullptr);
  clFinish(compute->queue);
}

void initParticleVBO(cl_GLuint *particleVBO, OpenCompute *compute, int numberOfParticles)
{
  glGenBuffers(1, particleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, *particleVBO);
  glBufferData(GL_ARRAY_BUFFER, numberOfParticles * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);
  compute->glParticleBuffer = clCreateFromGLBuffer(compute->context, CL_MEM_READ_WRITE, *particleVBO, nullptr);
  cout << "Particle Buffer: " << *particleVBO << endl;
  if (!compute->glParticleBuffer) { cerr << "Failed to create OpenCL buffer from OpenGL buffer." << endl; exit(1); }
}

void cleanUpOpenCl(OpenCompute *compute)
{
  clReleaseKernel(compute->kernel);
  clReleaseProgram(compute->program);
  clReleaseMemObject(compute->glParticleBuffer);
  clReleaseCommandQueue(compute->queue);
  clReleaseContext(compute->context);
}


int main(int argc, char **argv) {
  Shaders shaders;
  OpenCompute openCompute;
  cl_GLuint particleVBO;
  int numberOfParticles = errorManagment(argc, argv);
  clGetPlatformIDs(1, &openCompute.platform, nullptr);
  clGetDeviceIDs(openCompute.platform, CL_DEVICE_TYPE_GPU, 1, &openCompute.device, nullptr);
  openCompute.context = clCreateContext(nullptr, 1, &openCompute.device, nullptr, nullptr, nullptr);
  openCompute.queue = clCreateCommandQueue(openCompute.context, openCompute.device, 0, nullptr);
  GLFWwindow *window = glfwCreateWindow(1920, 1080, "Particle System", nullptr, nullptr);
  randomWindowManagment(window);
  initParticleVBO(&particleVBO, &openCompute, numberOfParticles);
  initOpenClProgram(&openCompute, numberOfParticles);
  shaders.shaderProgram = setupShaders(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader);
  if (shaders.shaderProgram == ShaderError) { glfwTerminate(); exit(-1);}
  glUseProgram(shaders.shaderProgram);
  mainLoop(window);
  cleanUp(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader, &shaders.shaderProgram);
  cleanUpOpenCl(&openCompute);
  glDeleteBuffers(1, &particleVBO);
  glfwTerminate();
  return 0;
}
