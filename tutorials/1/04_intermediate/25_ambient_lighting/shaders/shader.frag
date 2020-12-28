#version 330

in vec4 v_col;
in vec2 tex_coord;

out vec4 colour;

struct DirectionalLight {
  vec3 colour;
  float ambient_intensity;
};

uniform sampler2D the_texture;
uniform DirectionalLight directional_light;

void main() {
  vec4 ambient_colour = vec4(directional_light.colour, 1.0f) * directional_light.ambient_intensity;
  colour = texture(the_texture, tex_coord) * ambient_colour;
}