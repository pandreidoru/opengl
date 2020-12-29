//
// Created by pad on 12/29/20.
//
#pragma once
#include "light.h"

class PointLight : public Light {
 public:
  PointLight();
  PointLight(GLfloat red, GLfloat green, GLfloat blue,
             GLfloat ambient_intensity, GLfloat diffuse_intensity,
             GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
             GLfloat constant, GLfloat linear, GLfloat exponent);
  ~PointLight() override;

  void Use(GLuint ambient_intensity_location, GLuint ambient_colour_location,
           GLuint diffuse_intensity_location, GLuint position_location,
           GLuint constant_location, GLuint linear_location, GLuint exponent_location) const;

 private:
  glm::vec3 position_{0, 0, 0};

  // ax^2 + bx + c
  // x - distance from the source light to the fragment
  GLfloat constant_{1};  // c
  GLfloat linear_{0};    // b
  GLfloat exponent_{0};  // a
};
