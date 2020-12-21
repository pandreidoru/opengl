//
// Created by pad on 12/7/20.
//

#include "shader.h"

#include <cstring>

Shader::Shader() = default;

Shader::~Shader() {
  Clear();
}

void Shader::CreateFromString(char const* vert_code, char const* frag_code) {
  Compile(vert_code, frag_code);
}

void Shader::CreateFromFiles(char const* vert_file, char const* frag_file) {
  std::string vert_str = ReadFile(vert_file);
  std::string frag_str = ReadFile(frag_file);
  char const* vert_code = vert_str.c_str();
  char const* frag_code = frag_str.c_str();

  Compile(vert_code, frag_code);
}

std::string Shader::ReadFile(char const* file) {
  std::string content;
  std::ifstream fs(file, std::ios::in);

  if (!fs.is_open()) {
    std::cout << "Failed to read " << file << "!\n";
    return std::string();
  }

  std::string line{};
  while (!fs.eof()) {
    std::getline(fs, line);
    content.append(line + "\n");
  }

  fs.close();

  return content;
}

void Shader::Compile(char const* vertex_code, char const* fragment_code) {
  id_ = glCreateProgram();
  if (!id_) {
    std::cerr << "Error creating shader program\n";
    exit(EXIT_FAILURE);
  }

  Add(id_, vertex_code, GL_VERTEX_SHADER);
  Add(id_, fragment_code, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glLinkProgram(id_);
  glGetProgramiv(id_, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(id_, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error linking program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  glValidateProgram(id_);
  glGetProgramiv(id_, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(id_, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error validating program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  uniform_model_ = glGetUniformLocation(id_, "model");
  uniform_view_ = glGetUniformLocation(id_, "view");
  uniform_projection_ = glGetUniformLocation(id_, "projection");
}

void Shader::Add(GLuint program, char const* shader_code, GLenum type) {
  GLuint shader = glCreateShader(type);
  GLchar const* code[1];
  code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = std::strlen(shader_code);
  glShaderSource(shader, 1, code, code_length);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error compiling the " << type << " shader: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  glAttachShader(program, shader);
}

void Shader::Use() {
  glUseProgram(id_);
}

void Shader::Clear() {
  if (id_) {
    glDeleteProgram(id_);
    id_ = 0;
  }

  uniform_model_ = 0;
  uniform_view_ = 0;
  uniform_projection_ = 0;
}

GLuint Shader::GetModelLocation() {
  return uniform_model_;
}

GLuint Shader::GetViewLocation() {
  return uniform_view_;
}

GLuint Shader::GetProjectionLocation() {
  return uniform_projection_;
}
