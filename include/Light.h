#pragma once

#include "Buffer.h"
#include "ShaderProgram.h"
#include "VAO.h"

class Light {
public:
  Light(int columns, int rows, float radius, float frequency, float amplitude);

  int init();

  void draw();

  void destroy();

private:
  VAO m_vao;
  Buffer m_vbo;
  Buffer m_ebo;
  ShaderProgram m_program;

  size_t m_num_indices;

  int m_num_columns, m_num_rows;
  float m_radius;
  float m_frequency;
  float m_amplitude;
};
