#include "../includes/includes.hpp"

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

GLfloat cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
};

void mainLoop(GLFWwindow *window, Shaders *shaders) {
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaders->shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
  }
}


int main(int argc, char **argv) {
  Shaders shaders;
  int numberOfParticles = errorManagment(argc, argv);
  (void)numberOfParticles;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(1920, 1080, "Particle System", nullptr, nullptr);
  randomWindowManagment(window);
  shaders.shaderProgram = setupShaders(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader);
  if (shaders.shaderProgram == ShaderError) { glfwTerminate(); exit(-1);}
  glUseProgram(shaders.shaderProgram);
  mainLoop(window, &shaders);
  cleanUpShaders(&shaders.vertexShader, &shaders.fragmentShader, &shaders.computeShader, &shaders.shaderProgram);
  glfwTerminate();
  return 0;
}
