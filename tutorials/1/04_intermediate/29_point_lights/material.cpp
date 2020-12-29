//
// Created by pad on 12/29/20.
//

#include "material.h"

Material::Material() = default;

Material::Material(GLfloat specular_intensity, GLfloat shininess) :
    specular_intensity_{specular_intensity},
    shininess_{shininess} {}

void Material::Use(GLuint specular_intensity_location, GLuint shininess_location) {
  glUniform1f(specular_intensity_location, specular_intensity_);
  glUniform1f(shininess_location, shininess_);
}

Material::~Material() = default;
