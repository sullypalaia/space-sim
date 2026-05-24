#pragma once

#include <string>

class ShaderProgram {
public:
  ShaderProgram(const char *vert_filename, const char *frag_filename);

  int init();

  void use() const;

  void relink() const;

  void set_uniform1f(int location, float val) const;

  void set_uniform_matrix(int location, int count, unsigned char transpose,
                          const float *value) const;

  void set_uniform_block_binding(unsigned int block_index,
                                 unsigned int block_binding) const;

  void set_tf_varyings(int count, const char *const *varyings,
                       unsigned int buffer_mode);

  void destroy() const;

private:
  unsigned int m_id;

  std::string m_vert_filename;
  std::string m_frag_filename;
};
