//
// Created by pad on 12/29/20.
//
#pragma once
#include "light.h"

class DirectionalLight : public Light {
 public:
  DirectionalLight();
  DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                   GLfloat ambient_intensity,
                   GLfloat diffuse_intensity,
                   GLfloat x_dir, GLfloat y_dir, GLfloat z_dir);
  virtual ~DirectionalLight();

  void Use(GLuint ambient_intensity_location, GLuint ambient_colour_location,
           GLuint diffuse_intensity_location, GLuint direction_location) const;

 private:
  glm::vec3 direction_{0, -1, 0};
};
