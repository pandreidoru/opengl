//
// Created by pad on 12/6/20.
//
#include "mesh.h"

Mesh::Mesh() = default;

Mesh::~Mesh() {
  ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int vert_num, unsigned int indices_num) {
  index_count_ = indices_num;
  auto vert_size = sizeof(vertices[0]);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices_num, indices, GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vert_size * vert_num, vertices, GL_STATIC_DRAW);

  // Vertices coordinates
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vert_size * 5, nullptr);
  glEnableVertexAttribArray(0);

  // Texture coordinates
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vert_size * 5, (void*)(vert_size * 3));
  glEnableVertexAttribArray(1);

  // Unbind
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh() const {
  glBindVertexArray(vao_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::ClearMesh() {
  if (ibo_ != 0) {
    glDeleteBuffers(1, &ibo_);
    ibo_ = 0;
  }

  if (vbo_ != 0) {
    glDeleteBuffers(1, &vbo_);
    vbo_ = 0;
  }

  if (vao_ != 0) {
    glDeleteVertexArrays(1, &vao_);
    vao_ = 0;
  }

  index_count_ = 0;
}