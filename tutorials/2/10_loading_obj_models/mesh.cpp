//
// Created by pad on 1/3/21.
//
#include "mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

bool Mesh::LoadOBJ(std::string const& filename) {
  std::vector<unsigned int> vertex_indices, uv_indices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;

  if (filename.find(".obj") != std::string::npos) {
    std::ifstream fin(filename, std::ios::in);
    if (!fin) {
      std::cerr << "Cannot open " << filename << std::endl;
      return false;
    }

    std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

    std::string line_buffer;
    while (std::getline(fin, line_buffer)) {
      if (line_buffer.substr(0, 2) == "v ") {
        std::istringstream v(line_buffer.substr(2));
        glm::vec3 vertex;
        v >> vertex.x;
        v >> vertex.y;
        v >> vertex.z;
        temp_vertices.push_back(vertex);
      } else if (line_buffer.substr(0, 2) == "vt") {
        std::istringstream vt(line_buffer.substr(3));
        glm::vec2 uv;
        vt >> uv.s;
        vt >> uv.t;
        temp_uvs.push_back(uv);
      } else if (line_buffer.substr(0, 2) == "f ") {
        int p1, p2, p3; //to store mesh index
        int t1, t2, t3; //to store texture index
        int n1, n2, n3;
        std::istringstream face(line_buffer.substr(2));
        char tmp;

        // p1/t1/n1 p2/t2/n2 p3/t3/n3
        face >> p1 >> tmp >> t1 >> tmp >> n1
             >> p2 >> tmp >> t2 >> tmp >> n2
             >> p3 >> tmp >> t3 >> tmp >> n3;

        // We are ignoring normals (for now)

        vertex_indices.push_back(p1);
        vertex_indices.push_back(p2);
        vertex_indices.push_back(p3);

        uv_indices.push_back(t1);
        uv_indices.push_back(t2);
        uv_indices.push_back(t3);
      }
    }

    // Close the file
    fin.close();


    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertex_indices.size(); i++) {
      Vertex meshVertex;
      meshVertex.position_ = temp_vertices[vertex_indices[i] - 1];
      meshVertex.tex_coords_ = temp_uvs[uv_indices[i] - 1];

      vertices_.push_back(meshVertex);
    }

    // Create and initialize the buffers
    InitBuffers();

    return (loaded_ = true);
  }

  // We shouldn't get here so return failure
  return false;
}

void Mesh::Draw() {
  if (loaded_) {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    glBindVertexArray(0);
  }
}

void Mesh::InitBuffers() {
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);
  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);
  // Texture coords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}
