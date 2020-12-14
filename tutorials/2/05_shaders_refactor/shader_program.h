//
// Created by pad on 12/14/20.
//
#pragma once

#include <string>
#include <GL/glew.h>

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

 private:
  static std::string FileToString(std::string const &filename);
  void CheckCompileErrors(GLuint shader, ShaderType type) const;

  GLuint handle_{0};
};
