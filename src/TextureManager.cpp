#define STB_IMAGE_IMPLEMENTATION

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

#include "TextureManager.h"

TextureManager::TextureManager(bool flip) : m_flip(flip) {}

int TextureManager::init_cube_map(std::vector<const char *> &filenames) {
  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);

  int tex_width;
  int tex_height;
  int tex_num_channels;

  if (m_flip)
    stbi_set_flip_vertically_on_load(true);

  std::string full_path0 =
      static_cast<std::string>(std::filesystem::current_path()) + "/res/" +
      filenames[0];

  unsigned char *data0 = stbi_load(full_path0.data(), &tex_width, &tex_height,
                                   &tex_num_channels, 0);
  if (!data0) {
    std::cerr << "failed to load file " << full_path0.data() << '\n';
    return 0;
  }

  GLenum internal_format;
  GLenum sized_internal_format;

  if (!m_find_format(filenames[0], internal_format, sized_internal_format))
    return 0;

  glTextureStorage2D(m_id, 1, sized_internal_format, tex_width, tex_height);
  glTextureSubImage3D(m_id, 0, 0, 0, 0, tex_width, tex_height, 1,
                      internal_format, GL_UNSIGNED_BYTE, data0);

  for (int i = 1; i < 6; ++i) {
    std::string full_path =
        static_cast<std::string>(std::filesystem::current_path()) + "/res/" +
        filenames[i];

    unsigned char *data = stbi_load(full_path.data(), &tex_width, &tex_height,
                                    &tex_num_channels, 0);
    if (!data) {
      std::cerr << "failed to load file " << full_path.data() << '\n';
      return 0;
    }
    // assumes files are stored in order of px->nx->py->ny->pz->nz
    glTextureSubImage3D(m_id, 0, 0, 0, i, tex_width, tex_height, 1,
                        internal_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  return 1;
}

int TextureManager::init2d(std::vector<const char *> &filenames,
                           int num_textures) {
  m_ids.resize(num_textures);
  glCreateTextures(GL_TEXTURE_2D, filenames.size(), m_ids.data());
  for (int i = 0; i < filenames.size(); ++i) {
    std::string filename{filenames[i]};

    int tex_width;
    int tex_height;
    int tex_num_channels;

    std::string full_path =
        static_cast<std::string>(std::filesystem::current_path()) + "/res/" +
        filename;

    if (m_flip)
      stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(full_path.data(), &tex_width, &tex_height,
                                    &tex_num_channels, 0);

    if (!data) {
      std::cerr << "failed to load file " << full_path.data() << '\n';
      return 0;
    }

    // find the internal format
    GLenum internal_format;
    GLenum sized_internal_format;

    if (!m_find_format(filename, internal_format, sized_internal_format))
      return 0;

    glTextureStorage2D(m_ids[i], 1, sized_internal_format, tex_width,
                       tex_height);

    glTextureSubImage2D(m_ids[i], 0, 0, 0, tex_width, tex_height,
                        internal_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  return 1;
}

int TextureManager::init_materials(const aiScene *scene) {
  m_ids.reserve(scene->mNumMaterials);
  glCreateTextures(GL_TEXTURE_2D, scene->mNumMaterials, m_ids.data());

  for (int i = 0; i < scene->mNumMaterials; ++i) {
    unsigned char *data = nullptr;
    int tex_width;
    int tex_height;
    int tex_num_channels;

    aiMaterial *mat = scene->mMaterials[i];

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString path;
      if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) ==
          aiReturn_FAILURE) {
        std::cerr << "failed to get texture " << path.C_Str() << '\n';
        return 0;
      }

      if (static_cast<std::string>(path.C_Str()).substr(0, 1) == "*") {
        const aiTexture *texture = scene->GetEmbeddedTexture(path.C_Str());

        if (!texture) {
          std::cerr << "failed to get embedded texture " << path.C_Str()
                    << '\n';
        }

        if (texture->mHeight == 0) {
          data = stbi_load_from_memory(
              reinterpret_cast<const unsigned char *>(texture->pcData),
              texture->mWidth, &tex_width, &tex_height, &tex_num_channels, 4);
        } else {
          data = reinterpret_cast<unsigned char *>(texture->pcData);
          tex_width = texture->mWidth;
          tex_height = texture->mHeight;
          tex_num_channels = 4;
        }
      }
    }

    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTextureStorage2D(m_ids[i], 1, GL_RGBA8, tex_width, tex_height);
    glTextureSubImage2D(m_ids[i], 0, 0, 0, tex_width, tex_height, GL_RGBA,
                        GL_UNSIGNED_BYTE, data);

    if (data)
      stbi_image_free(data);
  }

  return 1;
}

void TextureManager::bind_texture_cube_map() const {
  glBindTextureUnit(0, m_id);
}

void TextureManager::bind_textures(size_t index) const {
  glBindTextureUnit(0, m_ids[index]);
}

void TextureManager::destroy() const {
  if (glIsTexture(m_id))
    glDeleteTextures(1, &m_id);
  else
    glDeleteTextures(m_ids.size(), m_ids.data());
}

int TextureManager::m_find_format(std::string_view filename,
                                  GLenum &internal_format,
                                  GLenum &sized_internal_format) {
  auto dot_pos = filename.find('.');
  std::string_view ext = filename.substr(dot_pos + 1);

  if (ext == "jpg" || ext == "jpeg") {
    internal_format = GL_RGB;
    sized_internal_format = GL_RGB8;
  } else if (ext == "png") {
    internal_format = GL_RGBA;
    sized_internal_format = GL_RGBA8;
  } else {
    std::cerr << "file extension " << ext << " not supported\n";
    return 0;
  }

  return 1;
}
