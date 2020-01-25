//
// Created by mrinaal on 1/25/20.
//

#ifndef OPENGLANDROID_RENDERERGL3_H
#define OPENGLANDROID_RENDERERGL3_H

// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif

#include <jni.h>

#include <vector>
#include <string>

#include "Texture.h"
#include "utils.h"

namespace ndk_opengl_app {

class Renderer {
 public:
  Renderer();

  ~Renderer();

  void Initialize();

  void RenderFrame();

  void UseProgram();

  void LoadTextureFromBitmap(JNIEnv* env, jobject bitmap);


 private:
  GLuint shader_program;

  GLuint vertex_array_obj;
  GLuint vertex_buffer_obj;
  GLuint element_buffer_obj;

  std::vector<Texture> texture_objs;
};

}   // namespace ndk_opengl_app

#endif //OPENGLANDROID_RENDERERGL3_H
