#include "glad/glad.h"

#include "VAO.h"

VAO::VAO() { glCreateVertexArrays(1, &m_id); }

void VAO::attach_vertex_buffer(Buffer &vbo, unsigned int binding_index,
                               int offset, long stride) const {
  glVertexArrayVertexBuffer(m_id, binding_index, vbo.get_id(), offset, stride);
}

void VAO::attach_element_buffer(Buffer &ebo) const {
  glVertexArrayElementBuffer(m_id, ebo.get_id());
}

void VAO::add_attribute(GLuint index, GLint size, GLenum type,
                        GLboolean normalized, GLuint relative_offset,
                        GLuint binding_index) const {
  glEnableVertexArrayAttrib(m_id, index);
  glVertexArrayAttribFormat(m_id, index, size, type, normalized,
                            relative_offset);
  glVertexArrayAttribBinding(m_id, index, binding_index);
}

void VAO::add_attribute_divisor(GLuint binding_index, GLuint divisor) const {
  glVertexArrayBindingDivisor(m_id, binding_index, divisor);
}

void VAO::bind() { glBindVertexArray(m_id); }

void VAO::destroy() const { glDeleteVertexArrays(1, &m_id); }
