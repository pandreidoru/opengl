//
// Created by pad on 12/30/20.
//
#pragma once

#include "point_light.h"

class SpotLight : PointLight {
 public:
  SpotLight();
  SpotLight(GLfloat red, GLfloat green, GLfloat blue,
            GLfloat ambient_intensity, GLfloat diffuse_intensity,
            GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
            GLfloat x_dir, GLfloat y_dir, GLfloat z_dir,
            GLfloat constant, GLfloat linear, GLfloat exponent,
            GLfloat edge);
  ~SpotLight() override;

  void Use(GLuint ambient_intensity_location, GLuint ambient_colour_location,
           GLuint diffuse_intensity_location, GLuint position_location, GLuint direction_location,
           GLuint constant_location, GLuint linear_location, GLuint exponent_location,
           GLuint edge_location) const;

  // Set light source to a new `position` with a specific `direction`
  void SetFlash(glm::vec3 position, glm::vec3 direction);

 private:
  glm::vec3 direction_{0, -1, 0};
  GLfloat edge_{0};  // the light angle in degrees
  GLfloat processed_edge_{0};  // the edge after processing: convert to radiant, apply cosine
};
