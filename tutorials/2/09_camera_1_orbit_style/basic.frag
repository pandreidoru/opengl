#version 330 core

in vec2 tex_coord_frag;
out vec4 frag_color;

uniform sampler2D my_texture1;
uniform sampler2D my_texture2;

void main() {
  frag_color = mix(texture(my_texture1, tex_coord_frag), texture(my_texture2, tex_coord_frag), 0.6);
}