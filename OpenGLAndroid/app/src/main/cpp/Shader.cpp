//
// Created by mrinaal on 3/1/20.
//

#include "Shader.h"

namespace ndk_opengl_app {

Shader::Shader(const char *vertex_shader_str, const char *fragment_shader_str) {
  const GLuint vertex_shader =
          LoadGLShader(GL_VERTEX_SHADER, vertex_shader_str);
  const GLuint fragment_shader =
          LoadGLShader(GL_FRAGMENT_SHADER, fragment_shader_str);

  glEnable(GL_DEPTH_TEST);

  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader);
  glAttachShader(program_id, fragment_shader);
  glLinkProgram(program_id);

  CheckGlError("Compile Shader program");

  // According to "https://learnopengl.com/Getting-started/Hello-Triangle"
  // it is okay to delete shader objects after linking
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}


void Shader::UseProgram() {
  glUseProgram(program_id);
}


// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glmath::Vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, value.GetData());
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glmath::Vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, value.GetData());
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glmath::Matrix4x4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, mat.ToGlArray().data());
}


void Shader::CheckGlError(const char* label) {
  int gl_error = glGetError();
  if (gl_error != GL_NO_ERROR) {
    LOGE("GL error @ %s: %d", label, gl_error);
    // Crash immediately to make OpenGL errors obvious.
    abort();
  }
}


GLuint Shader::LoadGLShader(GLenum type, const char* shader_source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  // Get the compilation status.
  GLint compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

  // If the compilation failed, delete the shader and show an error.
  if (compile_status == 0) {
    GLint info_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
    if (info_len == 0) {
      return 0;
    }

    std::vector<char> info_string(info_len);
    glGetShaderInfoLog(shader, info_string.size(), nullptr, info_string.data());
    LOGE("Could not compile shader of type %d: %s", type, info_string.data());
    glDeleteShader(shader);
    return 0;
  }
  else {
    return shader;
  }
}

}
