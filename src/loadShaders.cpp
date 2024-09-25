#include "../includes/includes.hpp"
#include <OpenGL/OpenGL.h>

GLuint loadShader(const char *filePath, GLenum shaderType) {
  std::ifstream shaderFile(filePath);
  if (!shaderFile.is_open()) {
    std::cerr << "Failed to open shader file: " << filePath << std::endl;
    return ShaderError;
  }
  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  std::string shaderCode = shaderStream.str();
  shaderFile.close();
  GLuint shader = glCreateShader(shaderType);
  const char *shaderCodeCStr = shaderCode.c_str();
  glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
  glCompileShader(shader);
  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE) {
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string errorLog(logLength, ' ');
    glGetShaderInfoLog(shader, logLength, nullptr, &errorLog[0]);
    std::cerr << "Shader compilation error: " << errorLog << std::endl;
    glDeleteShader(shader);
    return ShaderError;
  }
  return shader;
}

void cleanUpShaders(GLuint *vertexShader, GLuint *fragmentShader,
             GLuint *computeShader, GLuint *shaderProgram) {
  glfwTerminate();
  glDeleteShader(*vertexShader);
  glDeleteShader(*fragmentShader);
  glDeleteShader(*computeShader);
  glDeleteProgram(*shaderProgram);
}

GLuint setupShaders(GLuint *vertexShader, GLuint *fragmentShader,
                    GLuint *computeShader) {
  *vertexShader = loadShader("shaders/vertexShader.vert", GL_VERTEX_SHADER);
  if (*vertexShader == ShaderError) {
    cerr << "Failed to load vertexShader" << endl;
    return ShaderError;
  }
  *fragmentShader =
      loadShader("shaders/fragmentShader.frag", GL_FRAGMENT_SHADER);
  if (*fragmentShader == ShaderError) {
    cerr << "Failed to load fragmentShader" << endl;
    glDeleteShader(*vertexShader);
    return ShaderError;
  }
  *computeShader = loadShader("shaders/computeShader.comp", GL_COMPUTE_SHADER);
  if (*computeShader == ShaderError) {
    cerr << "Failed to load computeShader" << endl;
    glDeleteShader(*vertexShader);
    glDeleteShader(*fragmentShader);
    return ShaderError;
  }
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, *vertexShader);
  glAttachShader(shaderProgram, *fragmentShader);
  glAttachShader(shaderProgram, *computeShader);
  glLinkProgram(shaderProgram);
  GLint linkStatus;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
  if (linkStatus == GL_FALSE) {
    GLint logLength;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    string errorLog(logLength, ' ');
    glGetProgramInfoLog(shaderProgram, logLength, nullptr, &errorLog[0]);
    cerr << "Shader program linking error: " << errorLog << endl;
    cleanUpShaders(vertexShader, fragmentShader, computeShader, &shaderProgram);
    return ShaderError;
  }
  return shaderProgram;
}