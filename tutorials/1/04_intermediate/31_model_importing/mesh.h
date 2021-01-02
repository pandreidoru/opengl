//
// Created by pad on 12/6/20.
//
#pragma once

#include <GL/glew.h>

class Mesh {
 public:
  Mesh();
  ~Mesh();

  void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int vert_num, unsigned int indices_num);
  void RenderMesh() const;
  void ClearMesh();

 private:
  GLuint vao_{0};
  GLuint vbo_{0};
  GLuint ibo_{0};
  GLsizei index_count_{0};
};
