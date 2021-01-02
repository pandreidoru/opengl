//
// Created by pad on 12/27/20.
//
#pragma once

#include <GL/glew.h>
#include <stb_image.h>

class Texture {
 public:
  Texture();
  explicit Texture(char const* file_location);
  virtual ~Texture();

  bool Load();
  bool LoadA();
  void Use() const;
  void Clear();

 private:
  GLuint texture_id_{0};
  int width_{0};
  int height_{0};
  int bit_depth_{0};
  char const* file_location_{nullptr};
};
