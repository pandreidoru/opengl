//
// Created by pad on 12/7/20.
//
#pragma once

#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
 public:
  Shader();
  virtual ~Shader();

  void CreateFromString(char const* vert_code, char const* frag_code);
  void CreateFromFiles(char const* vert_file, char const* frag_file);

  std::string ReadFile(const char* file);

  GLuint GetModelLocation();
  GLuint GetProjectionLocation();

  void Use();
  void Clear();

 private:
  GLuint id_{0};
  GLuint uniform_model_{0};
  GLuint uniform_projection_{0};

  void Compile(char const* vertex_code, char const* fragment_code);
  void Add(GLuint program, char const* shader_code, GLenum type);
};
