//
// Created by pad on 12/21/20.
//

#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position,
               glm::vec3 up_world,
               GLfloat yaw,
               GLfloat pitch,
               GLfloat move_speed,
               GLfloat turn_speed) :
    position_{position}, up_world_{up_world},
    yaw_{yaw}, pitch_{pitch},
    move_speed_{move_speed}, turn_speed_{turn_speed} {
  Update();
}

Camera::~Camera() = default;

void Camera::KeyControl(bool const* keys, GLfloat delta_time) {
  GLfloat velocity = move_speed_ * delta_time;

  if (keys[GLFW_KEY_W]) {
    position_ += front_ * velocity;
  }

  if (keys[GLFW_KEY_S]) {
    position_ -= front_ * velocity;
  }

  if (keys[GLFW_KEY_A]) {
    position_ -= right_ * velocity;
  }

  if (keys[GLFW_KEY_D]) {
    position_ += right_ * velocity;
  }
}

void Camera::MouseControl(GLfloat x_change, GLfloat y_change) {
  x_change *= turn_speed_;
  y_change *= turn_speed_;

  yaw_ += x_change;
  pitch_ += y_change;

  // Limit the pitch movement such the target position is only front (ground <-> sky)
  pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);

  Update();
}

glm::vec3 Camera::GetCameraPosition() const {
  return position_;
}

glm::vec3 Camera::GetCameraDirection() const {
  return glm::normalize(front_);
}

glm::mat4 Camera::CalculateViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_world_);
}

void Camera::Update() {
  front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_.y = sin(glm::radians(pitch_));
  front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front_);  // Set magnitude to 1 unit

  right_ = glm::normalize(glm::cross(front_, up_world_));
  up_ = glm::normalize(glm::cross(right_, front_));
}
