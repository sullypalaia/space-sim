#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "ModelLoader.h"

ModelLoader::ModelLoader(const char *filename, const glm::mat4 &transform,
                         const ShaderProgram &program)
    : m_path(
          static_cast<std::string>(std::filesystem::current_path().string()) +
          "/res/" + filename),
      m_tex_manager(false), m_program(program), m_transform(transform) {}

int ModelLoader::init() {
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(m_path.data(), aiProcess_Triangulate);
  if (!scene) {
    std::cerr << importer.GetErrorString();
    return 0;
  }

  const int num_meshes = scene->mNumMeshes;

  m_meshes.reserve(num_meshes);

  int vertex_offset = 0;
  int index_offset = 0;

  for (int i = 0; i < num_meshes; ++i) {
    aiMesh *mesh = scene->mMeshes[i];

    int num_vertices = mesh->mNumVertices;
    int num_indices = mesh->mNumFaces * 3;

    m_meshes.push_back(
        {vertex_offset, index_offset, mesh->mMaterialIndex, num_indices});

    vertex_offset += num_vertices;
    index_offset += num_indices;

    for (int j = 0; j < mesh->mNumVertices; ++j) {
      glm::vec3 pos = {mesh->mVertices[j].x, mesh->mVertices[j].y,
                       mesh->mVertices[j].z};
      if (mesh->HasTextureCoords(0))
        m_vertices.push_back({pos, glm::vec2(mesh->mTextureCoords[0][j].x,
                                             mesh->mTextureCoords[0][j].y)});
      else
        m_vertices.push_back({pos, glm::vec2(0, 0)});
    }
    for (int j = 0; j < mesh->mNumFaces; ++j) {
      aiFace face = mesh->mFaces[j];

      for (int k = 0; k < face.mNumIndices; ++k)
        m_indices.push_back(face.mIndices[k]);
    }
  }

  if (!m_tex_manager.init_materials(scene))
    return 0;

  m_vbo.init(m_vertices.size() * sizeof(GLfloat) * 5, m_vertices.data(), 0);
  m_ebo.init(m_indices.size() * sizeof(GLuint), m_indices.data(), 0);
  if (!m_program.init())
    return 0;

  m_vao.attach_vertex_buffer(m_vbo, 0, 0, 5 * sizeof(GLfloat));
  m_vao.attach_element_buffer(m_ebo);

  m_vao.add_attribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  m_vao.add_attribute(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

  m_program.use();
  m_program.set_uniform_matrix(0, 1, GL_FALSE, glm::value_ptr(m_transform));

  return 1;
}

void ModelLoader::draw() {
  m_vao.bind();
  m_program.use();

  for (int i = 0; i < m_meshes.size(); ++i) {
    m_tex_manager.bind_textures(m_meshes[i].mat_index);
    glDrawElementsBaseVertex(
        GL_TRIANGLES, m_meshes[i].num_indices, GL_UNSIGNED_INT,
        reinterpret_cast<void *>(m_meshes[i].index_offset * sizeof(GLuint)),
        m_meshes[i].vertex_offset);
  }
}

void ModelLoader::destroy() const {
  m_vbo.destroy();
  m_ebo.destroy();
  m_vao.destroy();
  m_program.destroy();
}
