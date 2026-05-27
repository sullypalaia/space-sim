#include <iostream>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLFW/glfw3.h"

#include "Camera.h"
#include "ModelLoader.h"
#include "Planets.h"
#include "ShaderProgram.h"
#include "Skybox.h"
#include "Stars.h"
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
  Camera camera(45.0, window_manager.get_ar(), 5.0, 10000.0, 0.1, 80.0);

  window_manager.add_camera(&camera);

  // scene
  Planets planets(150, 50, 25, 20.0, 80.0);
  if (!planets.init()) {
    std::cerr << "failed to create planets\n";
    return -1;
  }

  // skybox
  Skybox skybox({"skybox/px.png", "skybox/nx.png", "skybox/py.png",
                 "skybox/ny.png", "skybox/pz.png", "skybox/nz.png"});
  if (!skybox.init()) {
    std::cerr << "failed to create skybox\n";
    return -1;
  }

  Stars stars(window_manager, 1000, 1.0, 10.0, -0.01, 0.01);
  if (!stars.init()) {
    std::cerr << "failed to create stars\n";
    return -1;
  }

  glm::mat4 spaceship_model_mat(1.0f);
  spaceship_model_mat =
      glm::translate(spaceship_model_mat, glm::vec3(0.0f, -1.0f, -5.0f));
  spaceship_model_mat = glm::scale(spaceship_model_mat, glm::vec3(0.0005f));

  ShaderProgram spaceship_program("spaceship.vert", "spaceship.frag");
  ShaderProgram other_models_program("model.vert", "model.frag");

  ModelLoader spaceship("spaceship.glb", spaceship_model_mat,
                        spaceship_program);
  if (!spaceship.init()) {
    std::cerr << "failed to create spaceship\n";
    return -1;
  }

  glm::mat4 deathstar_model_mat(1.0f);
  deathstar_model_mat =
      glm::translate(deathstar_model_mat, glm::vec3(400.0f, 800.0f, 800.0f));
  deathstar_model_mat = glm::scale(deathstar_model_mat, glm::vec3(5.0f));
  deathstar_model_mat = glm::rotate(deathstar_model_mat, glm::radians(-90.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f));

  ModelLoader deathstar("death-star.glb", deathstar_model_mat,
                        other_models_program);
  if (!deathstar.init()) {
    std::cerr << "failed to create deathstar\n";
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

    planets.draw();
    spaceship.draw();
    deathstar.draw();
    stars.draw();
    skybox.draw();

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean
  planets.destroy();
  skybox.destroy();
  stars.destroy();
  spaceship.destroy();
  deathstar.destroy();
  window_manager.destroy();
  glfwTerminate();

  return 0;
}
