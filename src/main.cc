#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "display.h"
#include "shader.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const char *VERTEX_PROGRAM = R"glsl(
#version 330 core
in vec2 position;
in vec3 color;

out vec3 out_color;

void main() {
    out_color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
)glsl";

const char *FRAGMENT_PROGRAM = R"glsl(
#version 330 core
in vec3 out_color;
out vec4 pixel_color;

void main() {
    pixel_color = vec4(out_color, 1.0);
}
)glsl";

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

  Shader shader = Shader(VERTEX_PROGRAM, FRAGMENT_PROGRAM);
  shader.Bind();

  float vertex_data[] = {-0.5, -0.5, 1.0, 0.0, 0.0,  // red
                         0.5,  -0.5, 0.0, 1.0, 0.0,  // green
                         0.0f, 0.5,  0.0, 0.0, 1.0}; // blue

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint pos_loc = shader.GetAttribLocation("position");
  glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  glEnableVertexAttribArray(pos_loc);

  GLuint color_loc = shader.GetAttribLocation("color");
  glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(color_loc);

  do {
    display->Clear(0.0, 0.0, 0.0, 1.0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    display->SwapBuffers();
    glfwPollEvents();
  } while (display->GetKey(GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           display->GetKey(GLFW_KEY_Q) != GLFW_PRESS &&
           !display->ShouldClose());

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vertex_buffer);
  glfwTerminate();
  return 0;
}
