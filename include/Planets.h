#pragma once

#include "Buffer.h"
#include "ShaderProgram.h"
#include "VAO.h"

class Planets {
public:
  Planets(int count, int num_columns, int num_rows, float min_radius,
          float max_radius);

  int init();

  void draw();

  void destroy();

private:
  VAO m_vao;
  Buffer m_vbo_pos;
  Buffer m_vbo_offsets_radii;
  Buffer m_ebo;
  ShaderProgram m_program;

  size_t m_num_indices;

  float m_min_radius;
  float m_max_radius;

  int m_num_columns;
  int m_num_rows;

  int m_count;
};
