//
// Created by pad on 12/30/20.
//
#include "spot_light.h"

SpotLight::SpotLight() = default;

SpotLight::SpotLight(GLfloat red,
                     GLfloat green,
                     GLfloat blue,
                     GLfloat ambient_intensity,
                     GLfloat diffuse_intensity,
                     GLfloat x_pos,
                     GLfloat y_pos,
                     GLfloat z_pos,
                     GLfloat x_dir,
                     GLfloat y_dir,
                     GLfloat z_dir,
                     GLfloat constant,
                     GLfloat linear,
                     GLfloat exponent,
                     GLfloat edge) :
    PointLight(red, green, blue,
               ambient_intensity, diffuse_intensity,
               x_pos, y_pos, z_pos,
               constant, linear, exponent),
    direction_(x_dir, y_dir, z_dir),
    edge_(edge),
    processed_edge_{cosf(glm::radians(edge_))} {}

SpotLight::~SpotLight() = default;

void SpotLight::Use(GLuint ambient_intensity_location,
                    GLuint ambient_colour_location,
                    GLuint diffuse_intensity_location,
                    GLuint position_location,
                    GLuint direction_location,
                    GLuint constant_location,
                    GLuint linear_location,
                    GLuint exponent_location,
                    GLuint edge_location) const {
  glUniform1f(ambient_intensity_location, ambient_intensity_);
  glUniform3f(ambient_colour_location, colour_.x, colour_.y, colour_.z);
  glUniform1f(diffuse_intensity_location, diffuse_intensity_);

  glUniform3f(position_location, position_.x, position_.y, position_.z);
  glUniform1f(constant_location, constant_);
  glUniform1f(linear_location, linear_);
  glUniform1f(exponent_location, exponent_);

  glUniform3f(direction_location, direction_.x, direction_.y, direction_.z);
  glUniform1f(edge_location, processed_edge_);
}
