#pragma once

#include <cstddef>

class Buffer {
public:
  void init(long size, void *data, unsigned int flags);

  unsigned int get_id() const;

  void bind_base(unsigned int target, unsigned int index);

  void add_subdata(std::ptrdiff_t offset, std::ptrdiff_t size, void *data);

  void destroy() const;

private:
  unsigned int m_id;
};
