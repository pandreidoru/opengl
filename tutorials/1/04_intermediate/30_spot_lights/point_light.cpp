//
// Created by pad on 12/29/20.
//

#include "point_light.h"

PointLight::PointLight() = default;

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat ambient_intensity, GLfloat diffuse_intensity,
                       GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
                       GLfloat constant, GLfloat linear, GLfloat exponent)
    : Light(red, green, blue, ambient_intensity, diffuse_intensity),
      position_(x_pos, y_pos, z_pos),
      constant_{constant},
      linear_{linear},
      exponent_{exponent} {}

PointLight::~PointLight() = default;

void PointLight::Use(GLuint ambient_intensity_location,
                     GLuint ambient_colour_location,
                     GLuint diffuse_intensity_location,
                     GLuint position_location,
                     GLuint constant_location,
                     GLuint linear_location,
                     GLuint exponent_location) const {
  glUniform1f(ambient_intensity_location, ambient_intensity_);
  glUniform3f(ambient_colour_location, colour_.x, colour_.y, colour_.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity_);
  glUniform3f(position_location, position_.x, position_.y, position_.z);
  glUniform1f(constant_location, constant_);
  glUniform1f(linear_location, linear_);
  glUniform1f(exponent_location, exponent_);
}

