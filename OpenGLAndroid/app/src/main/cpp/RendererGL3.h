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
#include "Camera.h"
#include "GLMath.h"
#include "Shader.h"
#include "utils.h"

namespace ndk_opengl_app {

class Renderer {
 public:
  Renderer();

  ~Renderer();

  void Initialize();

  void RenderFrame();

  void SetScreenParams(const int width, const int height);

  void LoadTextureFromBitmap(JNIEnv* env, jobject bitmap, int new_active_texture_id);

  void OnViewReset();

  void OnRotate(const float diff_x, const float diff_y);

  void OnTranslate(const float diff_x, const float diff_y);

  void OnScale(const float scale_x, const float scale_y);


 private:
  Shader* object_shader;
  Shader* light_shader;

  GLuint cubeVAO;
  GLuint lightVAO;
  GLuint vertex_buffer_obj;
  GLuint element_buffer_obj;

  std::vector<Texture> texture_objs;

  Camera* camera;

  int screen_width;
  int screen_height;
};

}   // namespace ndk_opengl_app

#endif //OPENGLANDROID_RENDERERGL3_H
