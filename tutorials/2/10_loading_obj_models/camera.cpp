//
// Created by pad on 12/20/20.
//

#include "camera.h"

#include <glm/gtx/transform.hpp>

Camera::Camera() = default;

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position_, target_pos_, up_);
}

OrbitCamera::OrbitCamera() = default;

void OrbitCamera::Rotate(float yaw, float pitch) {
  yaw_ = glm::radians(yaw);
  pitch_ = glm::radians(pitch);
  // Between (-90, 90) degrees <=> (-pi/2, pi/2) radians.
  // Add or subtract 0.1f to get to almost (-)90 degrees.
  pitch_ = glm::clamp(pitch_, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
  UpdateCameraVectors();
}

void OrbitCamera::SetLookAt(glm::vec3 const& target) {
  target_pos_ = target;
}

void OrbitCamera::SetRadius(float radius) {
  radius_ = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::UpdateCameraVectors() {
  // Spherical to Cartesian coordinates
  // https://en.wikipedia.org/wiki/Spherical_coordinate_system (NOTE: OpenGL coordinate sys has Y up, not Z)
  position_.x = target_pos_.x + radius_ * cosf(pitch_) * sinf(yaw_);
  position_.y = target_pos_.y + radius_ * sinf(pitch_);
  position_.z = target_pos_.z + radius_ * cosf(pitch_) * cosf(yaw_);
}

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) {
  position_ = position;
  yaw_ = yaw;
  pitch_ = pitch;
}

void FPSCamera::Move(glm::vec3 const& offset) {
  position_ += offset;
  UpdateCameraVectors();
}

void FPSCamera::Rotate(float yaw, float pitch) {
  yaw_ += glm::radians(yaw);
  pitch_ += glm::radians(pitch);

  // Constrain the pitch
  pitch_ = glm::clamp(pitch_, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
  UpdateCameraVectors();
}

void FPSCamera::UpdateCameraVectors() {
  // Calculate the view direction vector based on yaw and pitch angles (roll not considered)
  glm::vec3 look;
  look.x = cosf(pitch_) * sinf(yaw_);
  look.y = sinf(pitch_);
  look.z = cosf(pitch_) * cosf(yaw_);

  look_ = glm::normalize(look);

  // Re-calculate the Right and Up vector. For simplicity the Right vector will be assumed horizontal
  // w.r.t. the world's Up vector.
  right_ = glm::normalize(glm::cross(look_, world_up));
  up_ = glm::normalize(glm::cross(right_, look_));

  target_pos_ = position_ + look_;
}
