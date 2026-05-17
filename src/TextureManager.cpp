#define STB_IMAGE_IMPLEMENTATION

#include <filesystem>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

#include "TextureManager.h"

TextureManager::TextureManager(std::vector<const char *> &filenames, bool flip,
                               GLenum target)
    : m_filenames(filenames), m_flip(flip), m_target(target) {}

int TextureManager::init() {
  glCreateTextures(m_target, m_filenames.size(), m_ids.data());

  for (int i = 0; i < m_filenames.size(); ++i) {
    std::string filename{m_filenames[i]};

    int tex_width;
    int tex_height;
    int tex_num_channels;

    // find the full path
    std::string full_path =
        static_cast<std::string>(std::filesystem::current_path()) + '/' +
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
    std::string ext = filename.substr(filename.find('.'), filename.size() - 2);

    if (ext == "jpg" || ext == "jpeg") {
      internal_format = GL_RGB;
      sized_internal_format = GL_RGB8;
    } else if (ext == "png") {
      internal_format = GL_RGBA;
      sized_internal_format = GL_RGBA8;
    } else {
      std::cerr << "file extension not supported\n";
      return 0;
    }

    glTextureStorage2D(m_ids[i], 1, sized_internal_format, tex_width,
                       tex_height);

    // handle differently for different texture types
    if (m_target == GL_TEXTURE_2D) {
      glTextureSubImage2D(m_ids[i], 0, 0, 0, tex_width, tex_height,
                          internal_format, GL_UNSIGNED_BYTE, data);
    } else if (m_target == GL_TEXTURE_CUBE_MAP) {
      // assumes files are stored in order of px->nx->py->ny->pz->nz
      glTextureSubImage3D(m_ids[i], 0, 0, 0, 0, tex_width, tex_height, i,
                          internal_format, GL_UNSIGNED_BYTE, data);
    }
  }

  return 1;
}
