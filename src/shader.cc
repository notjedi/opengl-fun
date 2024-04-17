#include "shader.h"

Shader::Shader(const char *vert_shader_src, const char *frag_shader_src) {
  GLuint program = glCreateProgram();
  GLuint vert_shader = CreateShader(GL_VERTEX_SHADER, vert_shader_src);
  GLuint frag_shader = CreateShader(GL_FRAGMENT_SHADER, frag_shader_src);

  // bind "color" as the output variable which will be the color
  // glBindFragDataLocation(frag_shader, 0, "pixel_color");

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  glLinkProgram(program);
  CheckError(program, GL_LINK_STATUS, true, "Error while linking program");

  glValidateProgram(program);
  CheckError(program, GL_LINK_STATUS, true, "Invalid shader program");

  m_program = program;
  m_vert_shader = vert_shader;
  m_frag_shader = frag_shader;
}

Shader::~Shader() {
  GLuint shaders[] = {m_vert_shader, m_frag_shader};
  int len = sizeof(shaders) / sizeof(shaders[0]);
  for (int i = 0; i < len; i++) {
    glDetachShader(m_program, shaders[i]);
    glDeleteShader(shaders[i]);
  }
  glDeleteProgram(m_program);
}

void Shader::Bind() { glUseProgram(m_program); }

GLint Shader::GetAttribLocation(const char *name) {
  return glGetAttribLocation(m_program, name);
}

GLuint Shader::CreateShader(GLenum shader_type, const char *program) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &program, NULL);
  glCompileShader(shader);

  CheckError(shader, GL_COMPILE_STATUS, false, "Error while compiling shader");
  return shader;
}

GLint Shader::CheckError(GLuint shader, GLuint flag, bool is_program,
                         const char *err_msg) {
  GLint status;
  if (is_program) {
    glGetProgramiv(shader, flag, &status);
  } else {
    glGetShaderiv(shader, flag, &status);
  }

  if (status != GL_TRUE) {
    const int BUFFER_SIZE = 1024;
    char error[BUFFER_SIZE] = {0};
    if (is_program) {
      glGetProgramInfoLog(shader, BUFFER_SIZE, NULL, error);
    } else {
      glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, error);
    }
    fprintf(stderr, "%s: %s\n", err_msg, error);
  }
  return status;
}
