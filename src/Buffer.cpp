#include "glad/glad.h"

#include "Buffer.h"

void Buffer::init(GLsizeiptr size, void *data, GLbitfield flags) {
  glCreateBuffers(1, &m_id);
  glNamedBufferStorage(m_id, size, data, flags);
}

GLuint Buffer::get_id() const { return m_id; }

void Buffer::bind_base(GLenum target, GLuint index) {
  glBindBufferBase(target, index, m_id);
}

void Buffer::add_subdata(GLintptr offset, GLsizeiptr size, void *data) {
  glNamedBufferSubData(m_id, offset, size, data);
}

void Buffer::destroy() const { glDeleteBuffers(1, &m_id); }
