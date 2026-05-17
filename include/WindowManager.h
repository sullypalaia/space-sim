#pragma once

#include <string>

#include "Camera.h"
#include "GLFW/glfw3.h"

class WindowManager {
public:
  WindowManager(const char *name, const int width, const int height);

  int init();

  void add_camera(Camera *camera);

  GLFWwindow *get_window() const;

  float &get_ar();

  void handle_input();

  void destroy() const;

  float m_last_time = 0.0;
  float m_dt;

private:
  GLFWwindow *m_window = nullptr;

  std::string m_name;
  int m_width;
  int m_height;

  float m_ar;

  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

  static void resize_callback(GLFWwindow *window, int width, int height);

  static void cursor_pos_callback(GLFWwindow *window, double x, double y);

  Camera *m_camera = nullptr;
};
