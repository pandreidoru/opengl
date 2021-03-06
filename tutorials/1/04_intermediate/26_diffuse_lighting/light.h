//
// Created by pad on 12/28/20.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
 public:
  Light();
  Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambient_intensity,
        GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat diffuse_intensity);
  virtual ~Light();

  void Use(GLfloat ambient_colour_location, GLfloat ambient_intensity_location,
           GLfloat direction_location, GLfloat diffuse_intensity_location) const;

 private:
  glm::vec3 colour_{1.0f, 1.0f, 1.0f};  // how much of the texture colour will be visible
  GLfloat ambient_intensity_{1};

  glm::vec3 direction_{0.0f, -1.0f, 0.0f};
  GLfloat diffuse_intensity_{0};
};
