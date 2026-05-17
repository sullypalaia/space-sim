#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Light.h"
#include "WindowManager.h"

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar *message,
                    const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;

  std::cerr << "OPENGL DEBUG OUTPUT:\n"
            << "type: " << type << "\nseverity: " << severity << '\n'
            << message << '\n';
}

int main() {
  // glfw initialization
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // set up window manager
  WindowManager window_manager("space", 1920, 1080);
  if (!window_manager.init())
    return -1;

  // debugging
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(debug_callback, nullptr);

  // capabilities
  glEnable(GL_DEPTH_TEST);

  // camera
  Camera camera(45.0, window_manager.get_ar(), 0.01, 100.0, 0.05, 5.0);

  window_manager.add_camera(&camera);

  // scene
  Light light(50, 25, 1);
  if (!light.init()) {
    std::cerr << "failed to create light\n";
    return -1;
  }

  while (!glfwWindowShouldClose(window_manager.get_window())) {
    // black background
    constexpr GLfloat clear_color[4]{0.0f, 0.0f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, clear_color);

    constexpr GLfloat clear_depth = 1.0f;
    glClearBufferfv(GL_DEPTH, 0, &clear_depth);

    camera.update();
    window_manager.handle_input();

    // draw
    glDisable(GL_CULL_FACE);
    light.draw();

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean
  light.destroy();
  window_manager.destroy();
  glfwTerminate();

  return 0;
}
