//
// Created by pad on 12/20/20.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Camera {
 public:
  virtual void SetPosition(glm::vec3 const& position) { position_ = position; }
  virtual void Move(glm::vec3 const& offset) {}
  virtual void Rotate(float yaw, float pitch) {}  // Rotate camera along Y axe. In degrees.

  glm::mat4 GetViewMatrix() const;
  glm::vec3 const& GetLook() const { return look_; }
  glm::vec3 const& GetRight() const { return right_; }
  glm::vec3 const& GetUp() const { return up_; }

  float GetFOV() const { return fov_; }
  void SetFOV(float fov) { fov_ = fov; }  // In degrees

 protected:
  Camera();

  virtual void UpdateCameraVectors() {};

  glm::vec3 position_{0.0f, 0.0f, 0.0f};
  glm::vec3 target_pos_{0.0f, 0.0f, 0.0f};
  glm::vec3 up_{0.0f, 1.0f, 0.0f};
  glm::vec3 look_{0.0f, 0.0f, 0.0f}; // Z axe - Forward
  glm::vec3 right_{0.0f, 0.0f, 0.0f}; // X axe
  glm::vec3 const world_up{glm::vec3(0.0f, 1.0f, 0.0f)};
  float const kDefFOV{45.0};

  // Euler's angles - in radians (because GLM)
  float yaw_{glm::pi<float>()};
  float pitch_{0};

  float fov_{kDefFOV};  // Field Of View - in degrees
};

// It has a fixed point in space and orbits (rotate) around it.
class OrbitCamera : public Camera {
 public:
  OrbitCamera();

  void Rotate(float yaw, float pitch) override;  // In degrees

  // Camera controls
  void SetLookAt(glm::vec3 const& target);
  void SetRadius(float radius);

 private:
  void UpdateCameraVectors() override;

  float radius_{0};
};

class FPSCamera : public Camera {
 public:
  // (yaw) initial angle faces -Z
  explicit FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                     float yaw = glm::pi<float>(),
                     float pitch = 0.0f);

  void Move(glm::vec3 const& offset) override;
  void Rotate(float yaw, float pitch) override;

 protected:
  void UpdateCameraVectors() override;
};