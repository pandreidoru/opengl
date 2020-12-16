#version 330 core

in vec2 tex_coord_frag;
out vec4 frag_color;

uniform sampler2D my_texture;

void main() {
  frag_color = texture(my_texture, tex_coord_frag);
}