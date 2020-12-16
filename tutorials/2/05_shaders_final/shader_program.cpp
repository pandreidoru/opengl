//
// Created by pad on 12/14/20.
//

#include "shader_program.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram() = default;

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(handle_);
}

bool ShaderProgram::LoadShader(char const* vs_filename, char const* fs_filename) {
  std::string vs_string = FileToString(vs_filename);
  std::string fs_string = FileToString(fs_filename);
  GLchar const* vs_source_ptr = vs_string.c_str();
  GLchar const* fs_source_ptr = fs_string.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vs, 1, &vs_source_ptr, nullptr);
  glShaderSource(fs, 1, &fs_source_ptr, nullptr);

  glCompileShader(vs);
  CheckCompileErrors(vs, ShaderType::VERTEX);
  glCompileShader(fs);
  CheckCompileErrors(fs, ShaderType::FRAGMENT);

  handle_ = glCreateProgram();
  if (!handle_) {
    std::cerr << "Unable to create shader program!" << std::endl;
  }

  glAttachShader(handle_, vs);
  glAttachShader(handle_, fs);
  glLinkProgram(handle_);
  CheckCompileErrors(handle_, ShaderType::PROGRAM);

  uniform_locations_.clear();

  glDeleteShader(vs);
  glDeleteShader(fs);

  return true;
}

void ShaderProgram::Use() const {
  if (handle_) {
    glUseProgram(handle_);
  }
}

std::string ShaderProgram::FileToString(std::string const& filename) {
  std::stringstream ss;
  std::ifstream file;

  try {
    file.open(filename);

    if (!file.fail()) {
      ss << file.rdbuf();
    }
    file.close();
  } catch (std::exception& ex) {
    std::cerr << "Error reading shader " << filename << std::endl;
  }

  return ss.str();
}

void ShaderProgram::CheckCompileErrors(GLuint shader, ShaderProgram::ShaderType type) const {
  int status{0};
  if (type == ShaderType::PROGRAM) {
    glGetProgramiv(handle_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
      GLint length{0};
      glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length);

      std::string error_log(length, ' ');
      glGetProgramInfoLog(handle_, length, &length, &error_log[0]);
      std::cerr << "Error: Program failed to compile: " << error_log << std::endl;
    }
  } else {
    // VERTEX or FRAGMENT
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      GLint length{0};
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

      std::string error_log(length, ' ');
      glGetShaderInfoLog(shader, length, &length, &error_log[0]);
      std::cerr << "Error: Shader failed to compile: " << error_log << std::endl;
    }
  }
}

void ShaderProgram::SetUniform(GLchar const* name, glm::vec2 const& v) {
  GLint loc = GetUniformLocation(name);
  glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::SetUniform(GLchar const* name, glm::vec3 const& v) {
  GLint loc = GetUniformLocation(name);
  glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::SetUniform(GLchar const* name, glm::vec4 const& v) {
  GLint loc = GetUniformLocation(name);
  glUniform4f(loc, v.x, v.y, v.z, v.w);
}

GLint ShaderProgram::GetUniformLocation(GLchar const* name) {
  if (uniform_locations_.find(name) == uniform_locations_.end()) {
    uniform_locations_[name] = glGetUniformLocation(handle_, name);
  }

  return uniform_locations_[name];
}
