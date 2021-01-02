//
// Created by pad on 1/2/21.
//
#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "texture.h"

class Model {
 public:
  Model();
  virtual ~Model();

  void Load(std::string const& file_name);
  void Render();
  void Clear();

 private:
  void LoadNode(aiNode* node, aiScene const* scene);
  void LoadMesh(aiMesh* mesh, aiScene const* scene);
  void LoadMaterials(aiScene const* scene);

  std::vector<Mesh*> mesh_list_;
  std::vector<Texture*> texture_list_;
  std::vector<unsigned int> mesh_to_tex_;
};
