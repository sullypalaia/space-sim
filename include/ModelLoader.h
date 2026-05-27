#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Buffer.h"
#include "ShaderProgram.h"
#include "TextureManager.h"
#include "VAO.h"

struct Mesh {
  int vertex_offset;
  int index_offset;
  unsigned int mat_index;

  int num_indices;
};

struct Vertex {
  glm::vec3 pos;
  glm::vec2 tex_coord;
};

class ModelLoader {
public:
  ModelLoader(const char *filename, const glm::mat4 &transform,
              const ShaderProgram &program);

  int init();

  void draw();

  void destroy() const;

private:
  std::string m_path;

  std::vector<Mesh> m_meshes;

  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;

  TextureManager m_tex_manager;

  VAO m_vao;
  Buffer m_vbo;
  Buffer m_ebo;
  ShaderProgram m_program;

  const glm::mat4 &m_transform;
};
