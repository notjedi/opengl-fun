#pragma once

#include <cstdio>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display {
public:
  static std::unique_ptr<Display> create(int width, int height) {
    glewExperimental = GL_TRUE;
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti-aliasing
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (!window) {
      fprintf(stderr, "Could not create window\n");
      glfwTerminate();
      return nullptr;
    }

    glfwMakeContextCurrent(window);
    return std::unique_ptr<Display>(new Display(window));
  }

  ~Display();

  void SetInputMode(int mode, int value);
  int GetKey(int key);
  void Clear(float r, float g, float b, float a);
  void SwapBuffers();
  int ShouldClose();

private:
  Display(GLFWwindow *window);

  Display(const Display &){};
  void operator=(const Display &) {};

  GLFWwindow *m_window;
};
