//
// Created by mrinaal on 3/1/20.
//

#ifndef OPENGLANDROID_SHADER_H
#define OPENGLANDROID_SHADER_H

// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif


#include <cstdlib>
#include <vector>
#include <string>

#include "GLMath.h"
#include "utils.h"


namespace ndk_opengl_app {

class Shader {
 public:
  Shader(const char* vertex_shader_str, const char* fragment_shader_str);

  void UseProgram();

  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const;
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const;
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const;
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glmath::Vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glmath::Vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w) const;
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glmath::Matrix4x4 &mat) const;


 private:
  GLuint program_id;

  /**
   * Checks for OpenGL errors, and crashes if one has occurred.  Note that this
   * can be an expensive call, so real applications should call this rarely.
   *
   * @param label Error label
   */
  void CheckGlError(const char* label);


  /**
   * Converts a string into an OpenGL ES shader.
   *
   * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
   * @param shader_source The source code of the shader.
   * @return The shader object handler, or 0 if there's an error.
   */
  GLuint LoadGLShader(GLenum type, const char* shader_source);
};
}   // ndk_opengl_app

#endif //OPENGLANDROID_SHADER_H
