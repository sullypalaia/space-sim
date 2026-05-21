#pragma once

#include <vector>

class TextureManager {
public:
  TextureManager(bool flip, unsigned int target, size_t num_textures);

  int init(std::vector<const char *> &filenames);

  void bind_texture(size_t index) const;

  void destroy() const;

private:
  std::vector<unsigned int> m_ids;

  bool m_flip;

  unsigned int m_target;
};
