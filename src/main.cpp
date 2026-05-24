#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "Camera.h"
#include "WindowManager.h"

#include "Light.h"
#include "Skybox.h"
#include "Stars.h"

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
  Camera camera(45.0, window_manager.get_ar(), 0.01, 1000.0, 0.1, 20.0);

  window_manager.add_camera(&camera);

  // scene
  Light light(40, 20, 20, 10.0, 20.0);
  if (!light.init()) {
    std::cerr << "failed to create light\n";
    return -1;
  }

  // skybox
  Skybox skybox({"skybox/px.png", "skybox/nx.png", "skybox/py.png",
                 "skybox/ny.png", "skybox/pz.png", "skybox/nz.png"});
  if (!skybox.init()) {
    std::cerr << "failed to create skybox\n";
    return -1;
  }

  Stars stars(window_manager, 100, 1, 2, -0.1, 0.1);
  if (!stars.init()) {
    std::cerr << "failed to create stars\n";
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

    glEnable(GL_DEPTH_TEST);

    light.draw();

    stars.draw();

    skybox.draw();

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean
  light.destroy();
  skybox.destroy();
  stars.destroy();
  window_manager.destroy();
  glfwTerminate();

  return 0;
}
