#version 450 core

layout (location = 0) in vec2 angles_in;
layout (location = 1) in vec2 vel_in;

out vec2 angles_out;

layout (location = 0) uniform float dt;

float PI = 3.14159265;

void main() {
  vec2 angles = angles_in + vel_in * dt;

  if (angles.x > PI * 2.0)
    angles.x -= PI * 2.0;
  if (angles.x < 0)
    angles.x = 2.0 * PI - angles.x;

  if (angles.y > PI / 2.0)
    angles.y -= PI / 2.0;
  if (angles.y < PI / -2.0)
    angles.y += PI / 2.0;

  angles_out = angles_in + vel_in * dt;
}
