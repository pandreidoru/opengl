//
// Created by pad on 12/16/20.
//
#pragma once

#include <string>

#include <GL/glew.h>

class Texture2D {
 public:
  Texture2D();
  virtual ~Texture2D();

  bool Load(std::string const& filename, bool generate_mip_maps = true);
  void Bind(GLuint text_unit = 0);

 private:
  GLuint texture_{0};
};
