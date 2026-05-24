#include <random>
#include <utility>
#include <vector>

#include "glad/glad.h"

#include "Stars.h"

#define PI 3.14159265

Stars::Stars(const WindowManager &window_manager, int count, int min_size,
             int max_size, GLfloat min_vel, GLfloat max_vel)
    : m_count(count), m_sim_program("sim_stars.vert", ""),
      m_render_program("stars.vert", "stars.frag"), m_min_size(min_size),
      m_max_size(max_size), m_min_vel(min_vel), m_max_vel(max_vel),
      m_window_manager(window_manager) {}

int Stars::init() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist_theta(0, 2 * PI);
  std::uniform_real_distribution<float> dist_phi(-1.0 * (PI / 2.0), PI / 2.0);
  std::uniform_int_distribution dist_size(m_min_size, m_max_size);
  std::uniform_real_distribution<float> dist_vel(m_min_vel, m_max_vel);

  std::vector<GLfloat> angles;
  angles.reserve(m_count * 2);

  std::vector<GLfloat> vel;
  vel.reserve(m_count * 3);

  std::vector<int> size;
  size.reserve(m_count);

  for (int i = 0; i < m_count; ++i) {
    angles.push_back(dist_theta(mt));
    angles.push_back(dist_phi(mt));

    for (int k = 0; k < 2; ++k)
      vel.push_back(dist_vel(mt));

    size.push_back(dist_size(mt));
  }

  m_vbos_angles[m_read_index].init(angles.size() * sizeof(GLfloat),
                                   angles.data(), 0);
  m_vbos_angles[m_write_index].init(angles.size() * sizeof(GLfloat), nullptr,
                                    0);

  m_vbo_vel.init(vel.size() * sizeof(GLfloat), vel.data(), 0);

  m_vbo_size.init(size.size() * sizeof(GLint), size.data(), 0);

  glCreateTransformFeedbacks(2, m_tfs);

  const GLchar *const varyings[]{"angles_out"};

  if (!m_sim_program.init())
    return 0;
  if (!m_render_program.init())
    return 0;

  m_sim_program.set_tf_varyings(1, varyings, GL_INTERLEAVED_ATTRIBS);
  m_sim_program.relink();

  for (int i = 0; i < 2; ++i) {
    glTransformFeedbackBufferBase(m_tfs[i], 0, m_vbos_angles[i].get_id());

    m_vaos_sim[i].attach_vertex_buffer(m_vbos_angles[i], 0, 0,
                                       2 * sizeof(GLfloat));
    m_vaos_sim[i].attach_vertex_buffer(m_vbo_vel, 1, 0, 2 * sizeof(GLfloat));

    m_vaos_render[i].attach_vertex_buffer(m_vbos_angles[i], 0, 0,
                                          2 * sizeof(GLfloat));
    m_vaos_render[i].attach_vertex_buffer(m_vbo_size, 1, 0, sizeof(GLint));

    m_vaos_sim[i].add_attribute(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    m_vaos_sim[i].add_attribute(1, 2, GL_FLOAT, GL_FALSE, 0, 1);

    m_vaos_render[i].add_attribute(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    m_vaos_render[i].add_attribute(1, 1, GL_INT, GL_FALSE, 0, 1);
  }

  m_sim_program.set_uniform_block_binding(0, 0);

  glEnable(GL_PROGRAM_POINT_SIZE);

  return 1;
}

void Stars::draw() {
  m_sim_program.use();
  m_sim_program.set_uniform1f(0, m_window_manager.m_dt);

  m_vaos_sim[m_read_index].bind();
  glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfs[m_write_index]);

  glEnable(GL_RASTERIZER_DISCARD);

  glBeginTransformFeedback(GL_POINTS);
  glDrawArrays(GL_POINTS, 0, m_count);
  glEndTransformFeedback();

  glDisable(GL_RASTERIZER_DISCARD);

  m_vaos_render[m_write_index].bind();
  m_render_program.use();

  glDrawArrays(GL_POINTS, 0, m_count);

  std::swap(m_write_index, m_read_index);
}

void Stars::destroy() const {
  glDeleteTransformFeedbacks(2, m_tfs);
  m_vbo_size.destroy();
  m_vbo_vel.destroy();
  for (int i = 0; i < 2; ++i) {
    m_vaos_sim[i].destroy();
    m_vaos_render[i].destroy();
    m_vbos_angles[i].destroy();
  }
  m_sim_program.destroy();
  m_render_program.destroy();
}
