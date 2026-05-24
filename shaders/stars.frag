#version 450 core

out vec4 frag_color;

void main() {
  if (length(gl_PointCoord - vec2(0.5)) > 0.5)
    discard;

  frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}
