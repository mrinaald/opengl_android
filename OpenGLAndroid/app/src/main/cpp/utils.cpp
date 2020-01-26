//
// Created by mrinaal on 1/19/20.
//

#include "utils.h"

#include <android/log.h>
#include <string.h>  // Needed for strtok_r and strstr
#include <time.h>
#include <unistd.h>

#include <array>
#include <cmath>
#include <random>
#include <sstream>
#include <string>



namespace ndk_opengl_app {

void CheckGlError(const char* file, int line, const char* label) {
  int gl_error = glGetError();
  if (gl_error != GL_NO_ERROR) {
    LOGE("%s : %d > GL error @ %s: %d", file, line, label, gl_error);
    // Crash immediately to make OpenGL errors obvious.
    abort();
  }
}


GLuint LoadGLShader(GLenum type, const char* shader_source) {
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


long GetMonotonicTimeMilliSecs() {
  struct timespec res{};
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (res.tv_sec * 1000) + (res.tv_nsec / 1000000);
}

}   // namespace ndk_opengl_app

