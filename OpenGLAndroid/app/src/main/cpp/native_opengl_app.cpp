//
// Created by mrinaal on 1/19/20.
//

#include "native_opengl_app.h"

#include <android/log.h>

namespace ndk_opengl_app {

NativeOpenGLApp::NativeOpenGLApp(JavaVM *vm)
        : screen_width(0),
          screen_height(0),
          gesture_mode(MODE_ROTATE) {
  JNIEnv* env;
  vm->GetEnv((void**)&env, JNI_VERSION_1_6);
  renderer = new Renderer();
}


NativeOpenGLApp::~NativeOpenGLApp() {
  delete renderer;
}


void NativeOpenGLApp::OnPause() {}


void NativeOpenGLApp::OnResume() {}


void NativeOpenGLApp::OnSurfaceCreated(JNIEnv *env) {
  renderer->Initialize();
}


void NativeOpenGLApp::OnDrawFrame() {
  renderer->RenderFrame();
}


void NativeOpenGLApp::OnSurfaceChanged(int width, int height) {
  screen_width = width;
  screen_height = height;
  renderer->SetScreenParams(width, height);
}


void NativeOpenGLApp::LoadTextureFromBitmap(JNIEnv* env, jobject bitmap, int new_active_texture_id) {
  renderer->LoadTextureFromBitmap(env, bitmap, new_active_texture_id);
}


void NativeOpenGLApp::OnViewReset() {
  renderer->OnViewReset();
}


void NativeOpenGLApp::OnSwitchGestureMode(int mode) {
  switch (mode) {
    case MODE_ROTATE:
    case MODE_PAN:
      gesture_mode = mode;
      break;
    default:
      break;
  }
}


void NativeOpenGLApp::OnDragGesture(const float diff_x, const float diff_y) {
  if (gesture_mode == MODE_ROTATE) {
    renderer->OnRotate(diff_x, diff_y);
  }
  else if (gesture_mode == MODE_PAN) {
    renderer->OnTranslate(diff_x, diff_y);
  }
}


void NativeOpenGLApp::OnScaleGesture(const float scale_x, const float scale_y) {
  renderer->OnScale(scale_x, scale_y);
}

}   // namespace ndk_opengl_app
