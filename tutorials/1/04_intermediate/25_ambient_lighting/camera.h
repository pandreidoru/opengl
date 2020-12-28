//
// Created by pad on 12/21/20.
//
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera {
 public:
  Camera(glm::vec3 position,
         glm::vec3 up_world,
         GLfloat yaw,
         GLfloat pitch,
         GLfloat move_speed,
         GLfloat turn_speed);

  virtual ~Camera();

  void KeyControl(bool const* keys, GLfloat delta_time);
  void MouseControl(GLfloat x_change, GLfloat y_change);

  glm::mat4 CalculateViewMatrix();

 private:
  void Update();

  glm::vec3 position_{0.0f, 0.0f, 0.0f};
  glm::vec3 front_{0.0f, 0.0f, -1.0f};
  glm::vec3 right_{0.0f, 0.0f, 0.0f};
  glm::vec3 up_{0.0f, 1.0f, 0.0f};
  glm::vec3 up_world_{0.0f, 1.0f, 0.0f};

  GLfloat yaw_{-90};  // Left/Right
  GLfloat pitch_{0};  // Up/Down

  GLfloat move_speed_{0.1};
  GLfloat turn_speed_{1};
};
