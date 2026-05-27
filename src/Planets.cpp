#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#define PI 3.14159265f

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "Planets.h"

Planets::Planets(int count, int num_columns, int num_rows, float min_radius,
                 float max_radius)
    : m_count(count), m_program("planets.vert", "planets.frag"),
      m_num_columns(num_columns), m_num_rows(num_rows),
      m_min_radius(min_radius), m_max_radius(max_radius) {}

int Planets::init() {
  std::vector<GLfloat> vertices(m_num_columns * m_num_rows * 3);
  std::vector<GLuint> indices;

  for (int i = 0; i < m_num_rows; ++i) {
    float phi = (PI / 2.0f) - PI * (static_cast<float>(i) /
                                    static_cast<float>(m_num_rows - 1));

    for (int j = 0; j < m_num_columns; ++j) {
      float theta =
          (static_cast<float>(j) / static_cast<float>(m_num_columns)) *
          (2.0f * PI);

      int cv = i * m_num_columns + j;

      // generate vertices
      vertices[cv * 3] = std::cos(phi) * std::cos(theta);
      vertices[cv * 3 + 2] = std::cos(phi) * std::sin(theta);
      vertices[cv * 3 + 1] = std::sin(phi);

      // generate indices
      if (i < m_num_rows - 1) {
        int nj = (j + 1) % m_num_columns;

        int tl = i * m_num_columns + j;
        int tr = i * m_num_columns + nj;
        int bl = (i + 1) * m_num_columns + j;
        int br = (i + 1) * m_num_columns + nj;

        if (i != 0) {
          indices.push_back(tl);
          indices.push_back(bl);
          indices.push_back(tr);
        }
        if (i != m_num_rows - 2) {
          indices.push_back(tr);
          indices.push_back(bl);
          indices.push_back(br);
        }
      }
    }
  }

  std::random_device rd;
  std::mt19937 mt(rd());

  std::uniform_real_distribution<float> dist_offsets(-5000.0, 5000.0);
  std::uniform_real_distribution<float> dist_radii(m_min_radius, m_max_radius);
  std::uniform_real_distribution<float> dist_colors(0.0, 1.0);

  std::vector<GLfloat> offsets_radii_color;
  offsets_radii_color.reserve(m_count * 4);

  for (int i = 0; i < m_count; ++i) {
    for (int j = 0; j < 3; ++j)
      offsets_radii_color.push_back(dist_offsets(mt));
    offsets_radii_color.push_back(dist_radii(mt));
    for (int j = 0; j < 3; ++j)
      offsets_radii_color.push_back(dist_colors(mt));
  }

  m_num_indices = std::size(indices);

  m_vbo_pos.init(vertices.size() * sizeof(GLfloat), vertices.data(), 0);
  m_vbo_offsets_radii.init(offsets_radii_color.size() * sizeof(GLfloat),
                           offsets_radii_color.data(), 0);

  m_ebo.init(indices.size() * sizeof(GLuint), indices.data(), 0);

  m_vao.attach_vertex_buffer(m_vbo_pos, 0, 0, 3 * sizeof(GLfloat));
  m_vao.attach_vertex_buffer(m_vbo_offsets_radii, 1, 0, 7 * sizeof(GLfloat));

  m_vao.attach_element_buffer(m_ebo);

  m_vao.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  m_vao.add_attribute(1, 3, GL_FLOAT, GL_FALSE, 0, 1);
  m_vao.add_attribute_divisor(1, 1);

  m_vao.add_attribute(2, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 1);
  m_vao.add_attribute_divisor(2, 1);

  m_vao.add_attribute(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 1);
  m_vao.add_attribute_divisor(3, 1);

  if (!m_program.init())
    return 0;

  return 1;
}

void Planets::draw() {
  m_vao.bind();
  m_program.use();

  glDrawElementsInstanced(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0,
                          m_count);
}

void Planets::destroy() {
  m_vbo_pos.destroy();
  m_ebo.destroy();
  m_vao.destroy();
  m_program.destroy();
}
