#pragma once

#include <cstdio>

#include <GL/glew.h>

class Shader {
public:
  Shader(const char *vert_shader_src, const char *frag_shader_src);
  ~Shader();

  void Bind();
  GLint GetAttribLocation(const char *name);
  GLint GetUniformLocation(const char *name);

private:
  static const int NUM_SHADERS = 2;
  Shader(Shader &&) = default;
  Shader(const Shader &){};
  void operator=(const Shader &) {};

  GLint CheckError(GLuint shader, GLuint flag, bool is_program,
                   const char *err_msg);
  GLuint CreateShader(GLenum shader_type, const char *program);

  GLuint m_program;
  GLuint m_vert_shader;
  GLuint m_frag_shader;
};
