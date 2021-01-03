//
// Created by pad on 1/2/21.
//

#include "model.h"

#include <iostream>

Model::Model() {}

Model::~Model() {

}

void Model::Load(std::string const& file_name) {
  Assimp::Importer importer;
  aiScene const* scene = importer.ReadFile(file_name,
                                           aiProcess_Triangulate |
                                               aiProcess_FlipUVs |
                                               aiProcess_GenSmoothNormals |
                                               aiProcess_JoinIdenticalVertices);
  if (scene) {
    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
  } else {
    std::cerr << "Model " << file_name << " failed to load: " << importer.GetErrorString() << "\n";
  }
}

void Model::Render() {
  for (int i = 0; i < mesh_list_.size(); ++i) {
    unsigned int material_index = mesh_to_tex_[i];
    if (material_index < texture_list_.size() &&
        texture_list_[material_index]) {
      texture_list_[material_index]->Use();
    }
    mesh_list_[i]->RenderMesh();
  }
}

void Model::Clear() {
  for (int i = 0; i < mesh_list_.size(); ++i) {
    if (mesh_list_[i]) {
      delete mesh_list_[i];
      mesh_list_[i] = nullptr;
    }
  }

  for (int i = 0; i < texture_list_.size(); ++i) {
    if (texture_list_[i]) {
      delete texture_list_[i];
      texture_list_[i] = nullptr;
    }
  }
}

void Model::LoadNode(aiNode* node, aiScene const* scene) {
  for (size_t i = 0; i < node->mNumMeshes; ++i) {
    LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
  }

  for (int i = 0; i < node->mNumChildren; ++i) {
    LoadNode(node->mChildren[i], scene);
  }
}

// Load data in this format:
//   GLfloat vertices[] = {
//       //  x,     y,     z,     u,     v, normX, normY, normZ
//       -1.0f, -1.0f, -0.6f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//        0.0f, -1.0f,  1.0f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f,
//        1.0f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//        0.0f,  1.0f,  0.0f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f
//   };
void Model::LoadMesh(aiMesh* mesh, aiScene const* scene) {
  std::vector<GLfloat> vertices;
  std::vector<unsigned int> indices;

  // Load vertices in format: (x, y, z), (u, v), (normX, normY, normZ)
  for (int i = 0; i < mesh->mNumVertices; ++i) {
    vertices.insert(vertices.end(), {mesh->mVertices[i].x,
                                     mesh->mVertices[i].y,
                                     mesh->mVertices[i].z});
    // Get textures if exists
    if (mesh->mTextureCoords[0]) {
      vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x,
                                       mesh->mTextureCoords[0][i].y});
    } else {
      vertices.insert(vertices.end(), {0, 0});
    }

    // Get normals. They exists as we requested to be generated: `aiProcess_GenSmoothNormals`
    vertices.insert(vertices.end(), {-mesh->mNormals[i].x,
                                     -mesh->mNormals[i].y,
                                     -mesh->mNormals[i].z});
  }

  // Load indices (faces)
  for (int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  Mesh* new_mesh = new Mesh();
  new_mesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
  mesh_list_.push_back(new_mesh);
  mesh_to_tex_.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(aiScene const* scene) {
  texture_list_.resize(scene->mNumMaterials);

  for (int i = 0; i < scene->mNumMaterials; ++i) {
    aiMaterial* material = scene->mMaterials[i];
    texture_list_[i] = nullptr;

    if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
      aiString path;
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
        auto idx = std::string(path.data).rfind('\\');
        auto filename = std::string(path.data).substr(idx + 1);
        auto tex_path = std::string("../textures/") + filename;
        auto ext_start = tex_path.rfind('.');
        std::transform(tex_path.begin() + ext_start, tex_path.end(),
                       tex_path.begin() + ext_start, ::tolower);
        texture_list_[i] = new Texture(tex_path.c_str());
        if (!texture_list_[i]->Load()) {
          std::cerr << "Failed to load texture at " << tex_path << "\n";
          delete texture_list_[i];
          texture_list_[i] = nullptr;
        }
      }
    }

    if (!texture_list_[i]) {
      // texture_list_[i] = new Texture("../textures/brick.png");
      texture_list_[i] = new Texture("../textures/plain.png");
      texture_list_[i]->LoadA();
    }
  }
}
