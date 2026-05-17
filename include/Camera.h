#pragma once

#include "glm/glm.hpp"

#include "Buffer.h"

typedef struct {
  glm::mat4 view;
  glm::mat4 proj;
} CameraInfo;

class Camera {
public:
  Camera(float fovy, float &ar, float near, float far, float sensitivity,
         float speed);

  void update();

  float m_sensitivity;
  float m_speed;

  double m_yaw{-90.0};
  double m_pitch{0.0};

  glm::vec3 m_camera_front{0.0f, 0.0f, -1.0f};
  glm::vec3 m_camera_up{0.0f, 1.0f, 0.0f};
  glm::vec3 m_camera_pos{0.0f, 0.0f, 3.0f};

private:
  float m_fovy;
  float &m_ar;

  float m_near, m_far;

  Buffer m_ubo;
};
