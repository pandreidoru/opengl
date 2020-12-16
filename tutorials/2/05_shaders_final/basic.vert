#version 330 core

layout (location = 0) in vec3 pos;

uniform vec2 pos_offset;

void main() {
  gl_Position = vec4(pos.x + pos_offset.x, pos.y + pos_offset.y, pos.z, 1.0f);
}