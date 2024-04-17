#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "display.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main() {
  if (!glfwInit()) {
    fprintf(stderr, "Could not initialize glfw\n");
    return -1;
  }

  std::unique_ptr<Display> display =
      Display::create(WINDOW_WIDTH, WINDOW_HEIGHT);
  if (!display) {
    return 1;
  }
  display->SetInputMode(GLFW_STICKY_KEYS, GLFW_TRUE);

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Could not initialize glew\n");
    glfwTerminate();
    return -1;
  }

  do {
    display->Clear(1.0, 1.0, 1.0, 1.0);

    display->SwapBuffers();
    glfwPollEvents();
  } while (display->GetKey(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           display->GetKey(GLFW_KEY_Q) != GLFW_PRESS &&
           !display->ShouldClose());

  glfwTerminate();
  return 0;
}
