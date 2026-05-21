#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

#include "Camera.h"

Camera::Camera(float fovy, float &ar, float near, float far, float sensitivity,
               float speed)
    : m_fovy(fovy), m_ar(ar), m_near(near), m_far(far),
      m_sensitivity(sensitivity), m_speed(speed) {
  m_ubo.init(sizeof(CameraInfo), nullptr, GL_DYNAMIC_STORAGE_BIT);
  m_ubo.bind_base(GL_UNIFORM_BUFFER, 0);
}

void Camera::update() {
  glm::mat4 view(1.0f);
  view = glm::lookAt(m_camera_pos, m_camera_pos + m_camera_front, m_camera_up);

  glm::mat4 proj = glm::perspective(glm::radians(m_fovy), m_ar, m_near, m_far);

  m_ubo.add_subdata(0, sizeof(glm::mat4), glm::value_ptr(view));
  m_ubo.add_subdata(offsetof(CameraInfo, proj), sizeof(glm::mat4),
                    glm::value_ptr(proj));
}
