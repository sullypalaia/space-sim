#version 450 core

layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 offset_in;
layout (location = 2) in float radius_in;
layout (location = 3) in vec3 color_in;

layout (std140, binding = 0) uniform camera_info {
  mat4 view;
  mat4 proj;
};

out vec3 color;

void main() {
  color = color_in;

  gl_Position = proj * view * vec4((radius_in * pos_in + offset_in), 1.0);
}
