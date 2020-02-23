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

  void OnDragGesture(const float diff_x, const float diff_y);

  void OnScaleGesture(const float scale_x, const float scale_y);


 private:
  const static int MODE_ROTATE = 1;
  const static int MODE_PAN = 2;
  int gesture_mode;

  int screen_width;
  int screen_height;

  Renderer* renderer;
};

} // namespace ndk_opengl_app

#endif //OPENGLANDROID_NATIVE_OPENGL_APP_H
