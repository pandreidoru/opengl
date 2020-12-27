#version 330

in vec4 v_col;
in vec2 tex_coord;

out vec4 color;

uniform sampler2D the_texture;

void main() {
  color = texture(the_texture, tex_coord);
}