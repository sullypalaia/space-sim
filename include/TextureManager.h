#pragma once

#include <string_view>
#include <vector>

#include "assimp/scene.h"

class TextureManager {
public:
  TextureManager(bool flip);

  int init2d(std::vector<const char *> &filenames, int num_textures);
  int init_cube_map(std::vector<const char *> &filenames);
  int init_materials(const aiScene *scene);

  void bind_texture_cube_map() const;
  void bind_textures(size_t index) const;

  void destroy() const;

private:
  std::vector<unsigned int> m_ids;
  unsigned int m_id;

  bool m_flip;

  unsigned int m_target;

  int m_find_format(std::string_view filename, unsigned int &intenal_format,
                    unsigned int &sized_internal_format);
};
