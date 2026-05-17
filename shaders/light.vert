#version 450 core

layout (location = 0) in vec3 pos_in;

layout (std140, binding = 0) uniform camera_info {
  mat4 view;
  mat4 proj;
};

layout (location = 0) uniform mat4 model;

void main() {
  gl_Position = proj * view * model * vec4(pos_in, 1.0);
}
