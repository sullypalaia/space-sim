#pragma once

#include <array>
#include <vector>

#include "Buffer.h"
#include "ShaderProgram.h"
#include "TextureManager.h"
#include "VAO.h"

class Skybox {
public:
  Skybox(const std::array<const char *, 6> &filenames);

  int init();

  void draw();

  void destroy() const;

private:
  unsigned int m_id;

  std::vector<const char *> m_filenames;

  VAO m_vao;
  Buffer m_vbo;
  Buffer m_ebo;
  ShaderProgram m_program;
  TextureManager m_tex_manager;
};
