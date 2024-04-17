#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main() {
  if (!glfwInit()) {
    fprintf(stderr, "Could not initialize glfw\n");
    return 1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti-aliasing
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Could not create window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Could not initialize glew\n");
    glfwTerminate();
    return 1;
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

  do {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

  glfwTerminate();
  return 0;
}
