#define STB_IMAGE_IMPLEMENTATION

#include <filesystem>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

#include "TextureManager.h"

TextureManager::TextureManager(bool flip, GLenum target, size_t num_textures)
    : m_flip(flip), m_target(target) {
  if (m_target == GL_TEXTURE_2D)
    m_ids.resize(num_textures);
  else if (m_target == GL_TEXTURE_CUBE_MAP)
    m_ids.resize(1);
}

int TextureManager::init(std::vector<const char *> &filenames) {
  glCreateTextures(m_target, filenames.size(), m_ids.data());

  if (m_target == GL_TEXTURE_CUBE_MAP) {
    std::string filename = filenames[0];

    // get parameters from first file
    int tex_width;
    int tex_height;
    int tex_num_channels;

    // find the full path
    std::string full_path0 =
        static_cast<std::string>(std::filesystem::current_path()) + "/res/" +
        filename;

    if (m_flip)
      stbi_set_flip_vertically_on_load(true);

    unsigned char *data0 = stbi_load(full_path0.data(), &tex_width, &tex_height,
                                     &tex_num_channels, 0);

    if (!data0) {
      std::cerr << "failed to load file " << full_path0.data() << '\n';
      return 0;
    }

    // find the internal format
    GLenum internal_format;
    GLenum sized_internal_format;
    auto dot_pos = filename.find('.');
    std::string ext = filename.substr(dot_pos + 1);

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

    glTextureStorage2D(m_ids[0], 1, sized_internal_format, tex_width,
                       tex_height);

    glTextureSubImage3D(m_ids[0], 0, 0, 0, 0, tex_width, tex_height, 1,
                        internal_format, GL_UNSIGNED_BYTE, data0);

    for (int i = 1; i < filenames.size(); ++i) {
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
      glTextureSubImage3D(m_ids[0], 0, 0, 0, i, tex_width, tex_height, 1,
                          internal_format, GL_UNSIGNED_BYTE, data);
    }
  } else if (m_target == GL_TEXTURE_2D) {
    for (int i = 0; i < filenames.size(); ++i) {
      std::string filename{filenames[i]};

      int tex_width;
      int tex_height;
      int tex_num_channels;

      // find the full path
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
      auto dot_pos = filename.find('.');
      std::string ext = filename.substr(dot_pos + 1);

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

      glTextureStorage2D(m_ids[i], 1, sized_internal_format, tex_width,
                         tex_height);

      glTextureSubImage2D(m_ids[i], 0, 0, 0, tex_width, tex_height,
                          internal_format, GL_UNSIGNED_BYTE, data);

      stbi_image_free(data);
    }
  }

  if (m_target == GL_TEXTURE_2D) {
    for (int i = 0; i < filenames.size(); ++i) {
      // set up parameters
      glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTextureParameteri(m_ids[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTextureParameteri(m_ids[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  } else if (m_target == GL_TEXTURE_CUBE_MAP) {
    glTextureParameteri(m_ids[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ids[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ids[0], GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ids[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_ids[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  return 1;
}

void TextureManager::bind_texture(size_t index) const {
  glBindTextureUnit(0, m_ids[index]);
}

void TextureManager::destroy() const {
  glDeleteTextures(m_ids.size(), m_ids.data());
}
