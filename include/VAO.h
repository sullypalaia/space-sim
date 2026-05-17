#pragma once

#include "Buffer.h"

class VAO {
public:
  VAO();

  void attach_vertex_buffer(Buffer &vbo, unsigned int binding_index, int offset,
                            long stride) const;
  void attach_element_buffer(Buffer &ebo) const;

  void add_attribute(unsigned int index, int size, unsigned int type,
                     unsigned char normalized, unsigned int relative_offset,
                     unsigned int binding_index) const;

  void bind();

  void destroy() const;

private:
  unsigned int m_id;
};
