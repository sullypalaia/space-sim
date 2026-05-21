#version 450 core

layout (location = 0) in vec3 pos_in;

layout (std140, binding = 0) uniform CameraInfo {
  mat4 view;
  mat4 proj;
};

out vec3 tex_dirs;

void main() {
  tex_dirs = pos_in;

  vec4 pos = proj * mat4(mat3(view)) * vec4(pos_in, 1.0);
  gl_Position = pos.xyww;
}
