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
  void RenderMesh();
  void ClearMesh();

 private:
  GLuint VAO_{0};
  GLuint VBO_{0};
  GLuint IBO_{0};
  GLsizei index_count_{0};
};
