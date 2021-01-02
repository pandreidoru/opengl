//
// Created by pad on 12/27/20.
//

#include "texture.h"

#include <iostream>

Texture::Texture() = default;

Texture::Texture(char const* file_location) : file_location_{file_location} {}

Texture::~Texture() {
  Clear();
}

bool Texture::Load() {
  unsigned char* text_data = stbi_load(file_location_, &width_, &height_, &bit_depth_, 0);

  if (!text_data) {
    std::cerr << "Failed to find " << file_location_ << "\n";
    return false;
  }

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, text_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(text_data);
  return true;
}

bool Texture::LoadA() {
  unsigned char* text_data = stbi_load(file_location_, &width_, &height_, &bit_depth_, 0);

  if (!text_data) {
    std::cerr << "Failed to find " << file_location_ << "\n";
    return false;
  }

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, text_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(text_data);
  return true;
}

void Texture::Use() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::Clear() {
  glDeleteTextures(1, &texture_id_);
  texture_id_ = 0;
  width_ = 0;
  height_ = 0;
  bit_depth_ = 0;
  file_location_ = "";
}
