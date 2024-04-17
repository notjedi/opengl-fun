#include "display.h"

Display::Display(GLFWwindow *window) { m_window = window; }

Display::~Display() { glfwDestroyWindow(m_window); }

void Display::SetInputMode(int mode, int value) {
  glfwSetInputMode(m_window, mode, value);
}

int Display::GetKey(int key) { return glfwGetKey(m_window, key); }

void Display::Clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Display::SwapBuffers() { glfwSwapBuffers(m_window); }

int Display::ShouldClose() { return glfwWindowShouldClose(m_window); }
