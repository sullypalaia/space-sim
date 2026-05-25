#pragma once

#include "Buffer.h"
#include "ShaderProgram.h"
#include "VAO.h"
#include "WindowManager.h"

class Stars {
public:
  Stars(const WindowManager &window_manager, int count, float min_size,
        float max_size, float min_vel, float max_vel);

  int init();

  void draw();

  void destroy() const;

private:
  VAO m_vaos_sim[2];
  VAO m_vaos_render[2];
  Buffer m_vbos_angles[2];
  Buffer m_vbo_vel;
  Buffer m_vbo_size;

  ShaderProgram m_sim_program;
  ShaderProgram m_render_program;

  int m_count;

  float m_min_size;
  float m_max_size;

  float m_min_vel;
  float m_max_vel;

  unsigned int m_tfs[2];

  const WindowManager &m_window_manager;

  int m_read_index = 0;
  int m_write_index = 1;
};
