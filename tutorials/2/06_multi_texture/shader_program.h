//
// Created by pad on 12/14/20.
//
#pragma once

#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

class ShaderProgram {
 public:
  ShaderProgram();
  virtual ~ShaderProgram();

  enum class ShaderType {
    VERTEX,
    FRAGMENT,
    PROGRAM
  };

  bool LoadShader(char const *vs_filename, char const *fs_filename);
  void Use() const;

  void SetUniform(GLchar const* name, glm::vec2 const& v);
  void SetUniform(GLchar const* name, glm::vec3 const& v);
  void SetUniform(GLchar const* name, glm::vec4 const& v);
  GLuint GetProgram() const;

 private:
  static std::string FileToString(std::string const &filename);
  void CheckCompileErrors(GLuint shader, ShaderType type) const;
  GLint GetUniformLocation(GLchar const* name);

  GLuint handle_{0};
  std::map<std::string, GLint> uniform_locations_;
};
