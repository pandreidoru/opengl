//
// Created by pad on 12/29/20.
//

#include "directional_light.h"

DirectionalLight::DirectionalLight() = default;

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat ambient_intensity,
                                   GLfloat diffuse_intensity,
                                   GLfloat x_dir, GLfloat y_dir, GLfloat z_dir)
    : Light(red, green, blue,
            ambient_intensity,
            diffuse_intensity),
      direction_{x_dir, y_dir, z_dir} {}

void DirectionalLight::Use(GLuint ambient_intensity_location, GLuint ambient_colour_location,
                           GLuint diffuse_intensity_location, GLuint direction_location) const {
  glUniform1f(ambient_intensity_location, ambient_intensity_);
  glUniform3f(ambient_colour_location, colour_.x, colour_.y, colour_.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity_);
  glUniform3f(direction_location, direction_.x, direction_.y, direction_.z);
}

DirectionalLight::~DirectionalLight() = default;
