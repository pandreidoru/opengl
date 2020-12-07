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

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);

  glGenBuffers(1, &IBO_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices_num, indices, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vert_num, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh() {
  glBindVertexArray(VAO_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
  glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::ClearMesh() {
  if (IBO_ != 0) {
    glDeleteBuffers(1, &IBO_);
    IBO_ = 0;
  }

  if (VBO_ != 0) {
    glDeleteBuffers(1, &VBO_);
    VBO_ = 0;
  }

  if (VAO_ != 0) {
    glDeleteVertexArrays(1, &VAO_);
    VAO_ = 0;
  }

  index_count_ = 0;
}