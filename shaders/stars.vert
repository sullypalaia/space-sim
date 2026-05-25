#version 450 core

layout (location = 0) in vec2 angles_in;
layout (location = 1) in float size_in;

layout (std140, binding = 0) uniform camera_info {
  mat4 view;
  mat4 proj;
};

float dist = 800.0;

float pos_x = cos(angles_in.x) * cos(angles_in.y) * dist;
float pos_y = sin(angles_in.x) * dist;
float pos_z = cos(angles_in.x) * sin(angles_in.y) * dist;

void main() {
  gl_PointSize = size_in;

  gl_Position = proj * mat4(mat3(view)) * vec4(pos_x, pos_y, pos_z, 1.0);
}
