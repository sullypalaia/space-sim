#pragma once

#include <vector>

class TextureManager {
public:
  TextureManager(std::vector<const char *> &filenames, bool flip,
                 unsigned int target);

  int init();

  void destroy() const;

  void bind_texture() const;

private:
  std::vector<unsigned int> m_ids;

  std::vector<const char *> &m_filenames;

  bool m_flip;

  unsigned int m_target;
};
