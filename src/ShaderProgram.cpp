#include <filesystem>
#include <fstream>
#include <iostream>

#include "glad/glad.h"

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char *vert_filename,
                             const char *frag_filename = "")
    : m_vert_filename(vert_filename), m_frag_filename(frag_filename) {}

int ShaderProgram::init() {
  m_id = glCreateProgram();

  std::ifstream vert_in(
      static_cast<std::string>(std::filesystem::current_path()) + "/shaders/" +
      m_vert_filename);
  if (!vert_in) {
    std::cerr << "failed to open vertex shader " << m_vert_filename << '\n';
    return 0;
  }
  std::string vert_s{std::istreambuf_iterator<char>(vert_in),
                     std::istreambuf_iterator<char>()};
  const char *vert_cstr = vert_s.c_str();

  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vert_cstr, 0);
  glCompileShader(vert_shader);

  GLint vert_comp_status;
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_comp_status);

  if (!vert_comp_status) {
    std::cerr << "failed to compile vertex shader " << m_vert_filename << '\n';

    GLint vert_log_length;
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &vert_log_length);
    GLchar *vert_log =
        static_cast<GLchar *>(malloc(vert_log_length * sizeof(GLchar)));
    glGetShaderInfoLog(vert_shader, vert_log_length, NULL, vert_log);

    std::cerr << vert_log;
    return 0;
  }

  glAttachShader(m_id, vert_shader);
  glDeleteShader(vert_shader);

  // will not render when simulating
  if (!m_frag_filename.empty()) {
    std::ifstream frag_in(
        static_cast<std::string>(std::filesystem::current_path()) +
        "/shaders/" + m_frag_filename);
    if (!frag_in) {
      std::cerr << "failed to open fragment shader " << m_frag_filename << '\n';
    }
    std::string frag_s{std::istreambuf_iterator<char>(frag_in),
                       std::istreambuf_iterator<char>()};
    const char *frag_cstr = frag_s.c_str();

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_cstr, 0);
    glCompileShader(frag_shader);

    GLint frag_comp_status;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_comp_status);

    if (!frag_comp_status) {
      std::cerr << "failed to compile fragment shader " << m_frag_filename
                << '\n';

      GLint frag_log_length;
      glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &frag_log_length);
      GLchar *frag_log =
          static_cast<GLchar *>(malloc(frag_log_length * sizeof(GLchar)));
      glGetShaderInfoLog(frag_shader, frag_log_length, NULL, frag_log);

      std::cerr << frag_log;
      return 0;
    }

    glAttachShader(m_id, frag_shader);
    glDeleteShader(frag_shader);
  }

  glLinkProgram(m_id);
  GLint link_status;
  glGetProgramiv(m_id, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    std::cerr << "shader program linking failed\n";

    GLint program_log_length;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &program_log_length);
    GLchar *program_log =
        static_cast<GLchar *>(malloc(program_log_length * sizeof(GLchar)));
    glGetProgramInfoLog(m_id, program_log_length, NULL, program_log);

    std::cerr << program_log;
    return 0;
  }

  return 1;
}

void ShaderProgram::use() const { glUseProgram(m_id); }

void ShaderProgram::relink() const { glLinkProgram(m_id); }

void ShaderProgram::set_uniform1f(GLint location, GLfloat val) const {
  glUniform1f(location, val);
}

void ShaderProgram::set_uniform_matrix(GLint location, GLsizei count,
                                       GLboolean transpose,
                                       const GLfloat *value) const {
  glUniformMatrix4fv(location, count, transpose, value);
}

void ShaderProgram::set_uniform_block_binding(GLuint block_index,
                                              GLuint block_binding) const {
  glUniformBlockBinding(m_id, block_index, block_binding);
}

void ShaderProgram::set_tf_varyings(int count, const char *const *varyings,
                                    unsigned int buffer_mode) {
  glTransformFeedbackVaryings(m_id, count, varyings, buffer_mode);
}

void ShaderProgram::destroy() const { glDeleteProgram(m_id); }
