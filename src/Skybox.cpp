#include "glad/glad.h"

#include "Skybox.h"

Skybox::Skybox(const std::array<const char *, 6> &filenames)
    : m_program("skybox.vert", "skybox.frag"),
      m_tex_manager(false, GL_TEXTURE_CUBE_MAP, 6),
      m_filenames(filenames.begin(), filenames.end()) {}

int Skybox::init() {
  GLfloat pos[]{
      -1.0, -1.0, 1.0,  // 0 - front bottom left
      1.0,  -1.0, 1.0,  // 1 - front bottom right
      -1.0, 1.0,  1.0,  // 2 - front top left
      1.0,  1.0,  1.0,  // 3 - front top right
      1.0,  -1.0, -1.0, // 4 - back bottom right
      -1.0, -1.0, -1.0, // 5 - back bottom left
      1.0,  1.0,  -1.0, // 6 - back top right
      -1.0, 1.0,  -1.0  // 7 - back top left
  };

  GLuint indices[]{// front face
                   0, 1, 2, 2, 1, 3,

                   // back face
                   4, 5, 6, 6, 5, 7,

                   // right face
                   1, 4, 3, 3, 4, 6,

                   // left face
                   5, 0, 7, 7, 0, 2,

                   // top face
                   2, 3, 7, 7, 3, 6,

                   // bottom face
                   0, 1, 5, 5, 1, 4};

  // init pos
  m_vbo.init(sizeof(pos), pos, 0);
  m_vao.attach_vertex_buffer(m_vbo, 0, 0, 3 * sizeof(GLfloat));

  m_ebo.init(sizeof(indices), indices, 0);
  m_vao.attach_element_buffer(m_ebo);

  if (!m_program.init())
    return 0;

  // add position as vertex attribute
  m_vao.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  m_program.set_uniform_block_binding(0, 0);

  if (!m_tex_manager.init(m_filenames))
    return 0;

  return 1;

  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Skybox::draw() {
  m_vao.bind();
  m_program.use();
  m_tex_manager.bind_texture(0);

  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDepthFunc(GL_LESS);
}

void Skybox::destroy() const {
  m_vbo.destroy();
  m_ebo.destroy();
  m_vao.destroy();
  m_program.destroy();
}
