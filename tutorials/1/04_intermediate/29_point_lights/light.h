//
// Created by pad on 12/28/20.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light {
 public:
  Light();
  Light(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambient_intensity, GLfloat diffuse_intensity);
  virtual ~Light();

 protected:
  glm::vec3 colour_{1.0f, 1.0f, 1.0f};  // how much of the texture colour will be visible
  GLfloat ambient_intensity_{1};
  GLfloat diffuse_intensity_{0};
};
