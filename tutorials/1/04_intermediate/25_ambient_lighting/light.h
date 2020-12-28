//
// Created by pad on 12/28/20.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
 public:
  Light();
  Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambient_intensity);
  virtual ~Light();

  void Use(GLfloat ambient_colour_location, GLfloat ambient_intensity_location);

 private:
  glm::vec3 colour_{1, 1, 1};  // how much of the texture colour will be visible
  GLfloat ambient_intensity_{1};
};
