#version 450 core

out vec4 frag_color;

in vec3 tex_dirs;

uniform samplerCube sampler;

void main() {
  frag_color = texture(sampler, tex_dirs);
}
