#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265f

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW/glfw3.h"

#include "Light.h"

Light::Light(int num_columns, int num_rows, float radius)
    : m_program("light.vert", "light.frag"), m_num_columns(num_columns),
      m_num_rows(num_rows), m_radius(radius) {}

int Light::init() {
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
      vertices[cv * 3] = m_radius * std::cos(phi) * std::cos(theta);
      vertices[cv * 3 + 2] = m_radius * std::cos(phi) * std::sin(theta);
      vertices[cv * 3 + 1] = m_radius * std::sin(phi);

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

  m_num_indices = std::size(indices);

  m_vbo.init(vertices.size() * sizeof(GLfloat), vertices.data(), 0);
  m_ebo.init(indices.size() * sizeof(GLuint), indices.data(), 0);

  m_vao.attach_vertex_buffer(m_vbo, 0, 0, 3 * sizeof(GLfloat));
  m_vao.attach_element_buffer(m_ebo);

  // position
  m_vao.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  if (!m_program.init())
    return 0;

  // bind the uniform block
  m_program.set_uniform_block_binding(0, 0);

  return 1;
}

void Light::draw() {
  m_vao.bind();
  m_program.use();

  // set up model matrix
  glm::mat4 model_mat(1.0f);
  model_mat = glm::translate(
      model_mat,
      glm::vec3(std::cos(glm::radians(glfwGetTime() * 20.0f)) * 2.0f, 0.0f,
                std::sin(glm::radians(glfwGetTime() * 20.0f)) * 2.0f));
  m_program.set_uniform_matrix(0, 1, GL_FALSE, glm::value_ptr(model_mat));

  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, 0);
}

void Light::destroy() {
  m_vbo.destroy();
  m_ebo.destroy();
  m_vao.destroy();
  m_program.destroy();
}
