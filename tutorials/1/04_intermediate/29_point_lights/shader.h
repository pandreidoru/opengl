//
// Created by pad on 12/7/20.
//
#pragma once

#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "common_values.h"
#include "directional_light.h"
#include "point_light.h"

class Shader {
 public:
  Shader();
  virtual ~Shader();

  void CreateFromString(char const* vert_code, char const* frag_code);
  void CreateFromFiles(char const* vert_file, char const* frag_file);

  static std::string ReadFile(const char* file);

  GLuint GetModelLocation() const;
  GLuint GetViewLocation() const;
  GLuint GetProjectionLocation() const;
  GLuint GetAmbientColourLocation() const;
  GLuint GetAmbientIntensityLocation() const;
  GLuint GetDiffuseIntensityLocation() const;
  GLuint GetDirectionLocation() const;
  GLuint GetEyePosLocation() const;
  GLuint GetSpecularIntensityLocation() const;
  GLuint GetShininessLocation() const;

  void SetDirectionalLight(DirectionalLight* light) const;
  void SetPointLights(PointLight* lights, unsigned int count) const;

  void Use() const;
  void Clear();

 private:
  int point_light_count{0};

  GLuint id_{0};
  GLuint uniform_model_{0};
  GLuint uniform_view_{0};
  GLuint uniform_projection_{0};
  GLuint uniform_eye_pos_{0};
  GLuint uniform_specular_intensity_{0};
  GLuint uniform_shininess_{0};

  struct {
    GLuint uniform_colour_;
    GLuint uniform_ambient_intensity_;
    GLuint uniform_diffuse_intensity_;
    GLuint uniform_direction_;
  } uniform_directional_light_;

  GLuint uniform_point_light_count_{0};

  struct {
    GLuint uniform_colour_;
    GLuint uniform_ambient_intensity_;
    GLuint uniform_diffuse_intensity_;
    GLuint uniform_position_;
    GLuint uniform_constant_;
    GLuint uniform_linear_;
    GLuint uniform_exponent_;
  } uniform_point_light_[kMaxPointLights];

  void Compile(char const* vertex_code, char const* fragment_code);
  static void Add(GLuint program, char const* shader_code, GLenum type);
};
