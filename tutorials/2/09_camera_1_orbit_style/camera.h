//
// Created by pad on 12/20/20.
//

#pragma once

#include <glm/glm.hpp>

class Camera {
 public:
  glm::mat4 GetViewMatrix() const;

  // Rotate camera along Y axe.
  // The angles must be in degrees
  virtual void Rotate(float yaw, float pitch) {}

 protected:
  Camera();

  glm::vec3 position_{0.0f, 0.0f, 0.0f};
  glm::vec3 target_pos_{0.0f, 0.0f, 0.0f};
  glm::vec3 up_{0.0f, 1.0f, 0.0f};

  // Eulers angles (in radians because GLM)
  float yaw_{0};
  float pitch_{0};
};

// It has a fixed point in space and orbits (rotate) around it.
class OrbitCamera : public Camera {
 public:
  OrbitCamera();

  void Rotate(float yaw, float pitch) override;

  void SetLookAt(glm::vec3 const& target);
  void SetRadius(float radius);

 private:
  void UpdateCameraVectors();

  float radius_{0};
};
