//
// Created by pad on 12/28/20.
//
#include "light.h"

Light::Light() = default;

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambient_intensity,
             GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat diffuse_intensity)
    : colour_{red, green, blue}, ambient_intensity_{ambient_intensity},
      direction_{x_dir, y_dir, z_dir}, diffuse_intensity_{diffuse_intensity} {}

Light::~Light() = default;

void Light::Use(GLuint ambient_colour_location, GLuint ambient_intensity_location,
                GLuint direction_location, GLuint diffuse_intensity_location) const {
  glUniform3f(ambient_colour_location, colour_.x, colour_.y, colour_.z);
  glUniform1f(ambient_intensity_location, ambient_intensity_);
  glUniform3f(direction_location, direction_.x, direction_.y, direction_.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity_);
}
