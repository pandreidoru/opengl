//
// Created by pad on 12/29/20.
//
#pragma once

#include <GL/glew.h>

class Material {
 public:
  Material();
  Material(GLfloat specular_intensity, GLfloat shininess);
  virtual ~Material();

  void Use(GLuint specular_intensity_location, GLuint shininess_location);

 private:
  GLfloat specular_intensity_{0};
  GLfloat shininess_{0};
};
