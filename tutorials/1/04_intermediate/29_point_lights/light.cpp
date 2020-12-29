//
// Created by pad on 12/28/20.
//
#include "light.h"

Light::Light() = default;

Light::Light(GLfloat red, GLfloat green, GLfloat blue,
             GLfloat ambient_intensity, GLfloat diffuse_intensity)
    : colour_{red, green, blue},
      ambient_intensity_{ambient_intensity},
      diffuse_intensity_{diffuse_intensity} {}

Light::~Light() = default;
