//
// Created by pad on 12/28/20.
//

#include "light.h"

Light::Light() = default;

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambient_intensity)
    : colour_{red, green, blue}, ambient_intensity_(ambient_intensity) {}

Light::~Light() = default;

void Light::Use(GLfloat ambient_colour_location, GLfloat ambient_intensity_location) {
  glUniform3f(ambient_colour_location, colour_.x, colour_.y, colour_.z);
  glUniform1f(ambient_intensity_location, ambient_intensity_);
}
