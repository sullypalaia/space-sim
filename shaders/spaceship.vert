#version 450 core

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_coords_in;
//layout (location = 2) in vec3 normals_in;

out vec2 tex_coords;
//out vec3 normals;
//out vec3 frag_pos;

layout (std140, binding = 0) uniform camera_info {
  mat4 view;
  mat4 proj;
};

layout (location = 0) uniform mat4 model;

void main() {
  //vec4 frag = model * view * proj * vec4(pos_in, 1.0);
  //gl_Position = model * view * proj * vec4(pos_in, 1.0);
  gl_Position = proj * model * vec4(pos_in, 1.0);

  tex_coords = tex_coords_in;
  //normals = model * vec3(normals_in, 1.0);
}
