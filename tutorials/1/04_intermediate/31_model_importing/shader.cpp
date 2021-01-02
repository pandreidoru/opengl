//
// Created by pad on 12/7/20.
//

#include "shader.h"

#include <cstring>

Shader::Shader() = default;

Shader::~Shader() {
  Clear();
}

void Shader::CreateFromString(char const* vert_code, char const* frag_code) {
  Compile(vert_code, frag_code);
}

void Shader::CreateFromFiles(char const* vert_file, char const* frag_file) {
  std::string vert_str = ReadFile(vert_file);
  std::string frag_str = ReadFile(frag_file);
  char const* vert_code = vert_str.c_str();
  char const* frag_code = frag_str.c_str();

  Compile(vert_code, frag_code);
}

std::string Shader::ReadFile(char const* file) {
  std::string content;
  std::ifstream fs(file, std::ios::in);

  if (!fs.is_open()) {
    std::cout << "Failed to read " << file << "!\n";
    return std::string();
  }

  std::string line{};
  while (!fs.eof()) {
    std::getline(fs, line);
    content.append(line + "\n");
  }

  fs.close();

  return content;
}

void Shader::Compile(char const* vertex_code, char const* fragment_code) {
  id_ = glCreateProgram();
  if (!id_) {
    std::cerr << "Error creating shader program\n";
    exit(EXIT_FAILURE);
  }

  Add(id_, vertex_code, GL_VERTEX_SHADER);
  Add(id_, fragment_code, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glLinkProgram(id_);
  glGetProgramiv(id_, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(id_, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error linking program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  glValidateProgram(id_);
  glGetProgramiv(id_, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(id_, sizeof(result_log), nullptr, result_log);
    std::cerr << "Error validating program: " << result_log << "\n";
    exit(EXIT_FAILURE);
  }

  uniform_model_ = glGetUniformLocation(id_, "model");
  uniform_view_ = glGetUniformLocation(id_, "view");
  uniform_projection_ = glGetUniformLocation(id_, "projection");

  uniform_directional_light_.uniform_colour_ =
      glGetUniformLocation(id_, "directional_light.base.colour");
  uniform_directional_light_.uniform_ambient_intensity_ =
      glGetUniformLocation(id_, "directional_light.base.ambient_intensity");
  uniform_directional_light_.uniform_diffuse_intensity_ =
      glGetUniformLocation(id_, "directional_light.base.diffuse_intensity");
  uniform_directional_light_.uniform_direction_ =
      glGetUniformLocation(id_, "directional_light.direction");

  uniform_eye_pos_ = glGetUniformLocation(id_, "eye_pos");
  uniform_specular_intensity_ = glGetUniformLocation(id_, "material.specular_intensity");
  uniform_shininess_ = glGetUniformLocation(id_, "material.shininess");

  uniform_point_light_count_ = glGetUniformLocation(id_, "point_light_count");
  for (auto i = 0; i < kMaxPointLights; ++i) {
    char loc_buff[100] = {'\0'};
    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.colour", i);
    uniform_point_lights_[i].uniform_colour_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.ambient_intensity", i);
    uniform_point_lights_[i].uniform_ambient_intensity_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].base.diffuse_intensity", i);
    uniform_point_lights_[i].uniform_diffuse_intensity_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].position", i);
    uniform_point_lights_[i].uniform_position_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].constant", i);
    uniform_point_lights_[i].uniform_constant_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].linear", i);
    uniform_point_lights_[i].uniform_linear_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "point_lights[%d].exponent", i);
    uniform_point_lights_[i].uniform_exponent_ = glGetUniformLocation(id_, loc_buff);
  }

  uniform_spot_light_count_ = glGetUniformLocation(id_, "spot_light_count");
  for (auto i = 0; i < kMaxSpotLights; ++i) {
    char loc_buff[100] = {'\0'};
    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.colour", i);
    uniform_spot_lights_[i].uniform_colour_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.ambient_intensity", i);
    uniform_spot_lights_[i].uniform_ambient_intensity_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.base.diffuse_intensity", i);
    uniform_spot_lights_[i].uniform_diffuse_intensity_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.position", i);
    uniform_spot_lights_[i].uniform_position_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.constant", i);
    uniform_spot_lights_[i].uniform_constant_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.linear", i);
    uniform_spot_lights_[i].uniform_linear_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].base.exponent", i);
    uniform_spot_lights_[i].uniform_exponent_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].direction", i);
    uniform_spot_lights_[i].uniform_direction_ = glGetUniformLocation(id_, loc_buff);

    snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%d].edge", i);
    uniform_spot_lights_[i].uniform_edge_= glGetUniformLocation(id_, loc_buff);
  }
}

void Shader::Add(GLuint program, char const* shader_code, GLenum type) {
  GLuint shader = glCreateShader(type);
  GLchar const* code[1];
  code[0] = shader_code;

  GLint code_length[1];
  code_length[0] = std::strlen(shader_code);
  glShaderSource(shader, 1, code, code_length);

  GLint result = 0;
  GLchar result_log[1024] = {0};
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(shader, sizeof(result_log), nullptr, result_log);
    std::cout << "Error compiling the " << type << " shader: " << result_log << "\n";
    return;
  }

  glAttachShader(program, shader);
}

void Shader::SetDirectionalLight(DirectionalLight* light) const {
  light->Use(uniform_directional_light_.uniform_ambient_intensity_,
             uniform_directional_light_.uniform_colour_,
             uniform_directional_light_.uniform_diffuse_intensity_,
             uniform_directional_light_.uniform_direction_);
}

void Shader::SetPointLights(PointLight* lights, unsigned int count) const {
  if (count > kMaxPointLights) {
    count = kMaxPointLights;
  }

  glUniform1i(uniform_point_light_count_, count);

  for (auto i = 0; i < count; ++i) {
    lights[i].Use(uniform_point_lights_[i].uniform_ambient_intensity_,
                  uniform_point_lights_[i].uniform_colour_,
                  uniform_point_lights_[i].uniform_diffuse_intensity_,
                  uniform_point_lights_[i].uniform_position_,
                  uniform_point_lights_[i].uniform_constant_,
                  uniform_point_lights_[i].uniform_linear_,
                  uniform_point_lights_[i].uniform_exponent_);
  }
}

void Shader::SetSpotLights(SpotLight* lights, unsigned int count) const {
  if (count > kMaxSpotLights) {
    count = kMaxSpotLights;
  }

  glUniform1i(uniform_spot_light_count_, count);

  for (auto i = 0; i < count; ++i) {
    lights[i].Use(uniform_spot_lights_[i].uniform_ambient_intensity_,
                  uniform_spot_lights_[i].uniform_colour_,
                  uniform_spot_lights_[i].uniform_diffuse_intensity_,
                  uniform_spot_lights_[i].uniform_position_,
                  uniform_spot_lights_[i].uniform_direction_,
                  uniform_spot_lights_[i].uniform_constant_,
                  uniform_spot_lights_[i].uniform_linear_,
                  uniform_spot_lights_[i].uniform_exponent_,
                  uniform_spot_lights_[i].uniform_edge_);
  }
}

void Shader::Use() const {
  glUseProgram(id_);
}

void Shader::Clear() {
  if (id_) {
    glDeleteProgram(id_);
    id_ = 0;
  }

  uniform_model_ = 0;
  uniform_view_ = 0;
  uniform_projection_ = 0;
}

GLuint Shader::GetModelLocation() const {
  return uniform_model_;
}

GLuint Shader::GetViewLocation() const {
  return uniform_view_;
}

GLuint Shader::GetProjectionLocation() const {
  return uniform_projection_;
}

GLuint Shader::GetAmbientColourLocation() const {
  return uniform_directional_light_.uniform_colour_;
}

GLuint Shader::GetAmbientIntensityLocation() const {
  return uniform_directional_light_.uniform_ambient_intensity_;
}

GLuint Shader::GetDiffuseIntensityLocation() const {
  return uniform_directional_light_.uniform_diffuse_intensity_;
}

GLuint Shader::GetDirectionLocation() const {
  return uniform_directional_light_.uniform_direction_;
}

GLuint Shader::GetEyePosLocation() const {
  return uniform_eye_pos_;
}

GLuint Shader::GetSpecularIntensityLocation() const {
  return uniform_specular_intensity_;
}

GLuint Shader::GetShininessLocation() const {
  return uniform_shininess_;
}
