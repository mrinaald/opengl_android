//
// Created by mrinaal on 1/19/20.
//

#ifndef OPENGLANDROID_NATIVE_OPENGL_APP_H
#define OPENGLANDROID_NATIVE_OPENGL_APP_H

#include <jni.h>

#include "RendererGL3.h"

#include "utils.h"

namespace ndk_opengl_app {

class NativeOpenGLApp {
 public:
  NativeOpenGLApp(JavaVM* vm);

  ~NativeOpenGLApp();

  void OnPause();

  void OnResume();

  void OnSurfaceCreated(JNIEnv* env);

  void OnDrawFrame();

  void OnSurfaceChanged(int width, int height);

  void LoadTextureFromBitmap(JNIEnv* env, jobject bitmap, int new_active_texture_id);

  void OnViewReset();

  void OnSwitchGestureMode(int mode);


 private:
  int screen_width;
  int screen_height;

  Renderer* renderer;
};

} // namespace ndk_opengl_app

#endif //OPENGLANDROID_NATIVE_OPENGL_APP_H
