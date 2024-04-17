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

  glewExperimental = GL_TRUE;
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x anti-aliasing
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  float vertex_data[] = {-1.0, -1.0, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), &vertex_data,
               GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLchar *vertex_shader_prog = R"glsl(
    #version 330 core
    in vec3 pos;

    void main() {
        gl_Position = vec4(pos, 1.0);
    }
  )glsl";
  GLchar *vert_progs[] = {vertex_shader_prog};

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, vert_progs, NULL);
  glCompileShader(vertex_shader);

  GLint status;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    char buffer[1024];
    glGetShaderInfoLog(vertex_shader, 1024, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);
    fflush(stderr);
  }

  glGetShaderiv(vertex_shader, GL_VALIDATE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[1024];
    glGetShaderInfoLog(vertex_shader, 1024, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);
    fflush(stderr);
  }

  GLchar *frag_shader_prog = R"glsl(
    #version 330 core
    out vec4 pixel_color;

    void main() {
        pixel_color = vec4(1.0, 1.0, 1.0, 1.0);
    }
  )glsl";
  GLchar *frag_progs[] = {frag_shader_prog};

  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, frag_progs, NULL);
  glCompileShader(frag_shader);

  GLuint shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vertex_shader);
  glAttachShader(shader_prog, frag_shader);
  glBindFragDataLocation(frag_shader, 0, "pixel_color");
  glLinkProgram(shader_prog);
  glUseProgram(shader_prog);

  GLuint pos_attrib = glGetAttribLocation(shader_prog, "pos");
  glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(pos_attrib);

  do {
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
           !glfwWindowShouldClose(window));

  glfwTerminate();
  return 0;
}
