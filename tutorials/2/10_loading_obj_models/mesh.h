//
// Created by pad on 1/3/21.
//
#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position_{0, 0, 0};
  glm::vec2 tex_coords_{0, 0};
};

class Mesh {
 public:
  virtual ~Mesh();
  bool LoadOBJ(std::string const& filename);
  void Draw();

 private:
  void InitBuffers();

  bool loaded_{false};
  std::vector<Vertex> vertices_{};
  GLuint vao_{0};
  GLuint vbo_{0};
};
