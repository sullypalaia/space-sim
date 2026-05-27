#include <cmath>
#include <iostream>

#include "glad/glad.h"

#include "WindowManager.h"

WindowManager::WindowManager(const char *name, const int width,
                             const int height)
    : m_width(width), m_height(height), m_name(name) {}

int WindowManager::init() {
  m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
  if (!m_window) {
    std::cerr << "window creation failed\n";
    return 0;
  }

  glfwMakeContextCurrent(m_window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetWindowUserPointer(m_window, this);

  glfwSetKeyCallback(m_window, key_callback);
  glfwSetFramebufferSizeCallback(m_window, resize_callback);
  glfwSetCursorPosCallback(m_window, cursor_pos_callback);

  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glViewport(0, 0, m_width, m_height);

  m_ar = static_cast<float>(m_width) / static_cast<float>(m_height);

  return 1;
}

void WindowManager::add_camera(Camera *camera) { m_camera = camera; }

GLFWwindow *WindowManager::get_window() const { return m_window; }

float &WindowManager::get_ar() { return m_ar; }

void WindowManager::handle_input() {
  // handle delta time
  float curr_time = glfwGetTime();
  m_dt = curr_time - m_last_time;
  m_last_time = curr_time;

  m_camera->m_camera_front =
      glm::vec3(std::cos(glm::radians(m_camera->m_pitch)) *
                    std::cos(glm::radians(m_camera->m_yaw)),
                std::sin(glm::radians(m_camera->m_pitch)),
                std::cos(glm::radians(m_camera->m_pitch)) *
                    std::sin(glm::radians(m_camera->m_yaw)));

  if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
    m_camera->m_camera_pos +=
        m_camera->m_camera_front * m_camera->m_speed * m_dt;
  }
  if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
    m_camera->m_camera_pos -=
        m_camera->m_camera_front * m_camera->m_speed * m_dt;
  }
  if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
    m_camera->m_camera_pos +=
        glm::normalize(
            glm::cross(m_camera->m_camera_front, m_camera->m_camera_up)) *
        m_camera->m_speed * m_dt;
  }
  if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
    m_camera->m_camera_pos -=
        glm::normalize(
            glm::cross(m_camera->m_camera_front, m_camera->m_camera_up)) *
        m_camera->m_speed * m_dt;
  }
  if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    m_camera->m_camera_pos += m_camera->m_camera_up * m_camera->m_speed * m_dt;
  }
  if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    m_camera->m_camera_pos -= m_camera->m_camera_up * m_camera->m_speed * m_dt;
  }
}

void WindowManager::destroy() const { glfwDestroyWindow(m_window); }

void WindowManager::key_callback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void WindowManager::resize_callback(GLFWwindow *window, int width, int height) {
  auto *window_ptr =
      static_cast<WindowManager *>(glfwGetWindowUserPointer(window));

  window_ptr->m_ar = static_cast<float>(width) / static_cast<float>(height);

  glViewport(0, 0, width, height);
}

void WindowManager::cursor_pos_callback(GLFWwindow *window, double x,
                                        double y) {
  static double last_x = 500.0;
  static double last_y = 500.0;

  if (last_x == 500.0) {
    last_x = x;
    last_y = y;
    return;
  }

  auto window_ptr =
      static_cast<WindowManager *>(glfwGetWindowUserPointer(window));

  double yaw =
      window_ptr->m_camera->m_yaw +
      (x - last_x) * static_cast<double>(window_ptr->m_camera->m_sensitivity);

  double pitch =
      window_ptr->m_camera->m_pitch +
      (last_y - y) * static_cast<double>(window_ptr->m_camera->m_sensitivity);

  if (pitch > 89.0)
    pitch = 89.0;
  if (pitch < -89.0)
    pitch = -89.0;

  if (yaw > 360.0)
    yaw = 360.0 - yaw;
  if (yaw < -360.0)
    yaw += 360;

  window_ptr->m_camera->m_yaw = yaw;
  window_ptr->m_camera->m_pitch = pitch;

  last_x = x;
  last_y = y;
}
