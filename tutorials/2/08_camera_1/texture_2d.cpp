//
// Created by pad on 12/16/20.
//

#include "texture_2d.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture2D::Texture2D() = default;

Texture2D::~Texture2D() = default;

bool Texture2D::Load(std::string const& filename, bool generate_mip_maps) {
  int width, height, components;
  unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);

  if (!image_data) {
    std::cerr << "Error loading texture: " << filename << std::endl;
    return false;
  }

  // Invert image
  int width_bytes = width * 4;
  unsigned char* top{nullptr};
  unsigned char* bottom{nullptr};
  unsigned char temp = 0;
  int half_height = height / 2;
  for (int row = 0; row < half_height; row++) {
    top = image_data + row * width_bytes;
    bottom = image_data + (height - row - 1) * width_bytes;
    for (int col = 0; col < width_bytes; col++) {
      temp = *top;
      *top = *bottom;
      *bottom = temp;
      top++;
      bottom++;
    }
  }

  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

  if (generate_mip_maps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  stbi_image_free(image_data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}

void Texture2D::Bind(GLuint text_unit) {
  glActiveTexture(GL_TEXTURE0 + text_unit);
  glBindTexture(GL_TEXTURE_2D, texture_);
}
