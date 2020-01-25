//
// Created by mrinaal on 1/19/20.
//

#include "native_opengl_app.h"

#include <android/log.h>

namespace ndk_opengl_app {

NativeOpenGLApp::NativeOpenGLApp(JavaVM *vm)
        : screen_width(0),
          screen_height(0) {
  JNIEnv* env;
  vm->GetEnv((void**)&env, JNI_VERSION_1_6);
}


NativeOpenGLApp::~NativeOpenGLApp() {}


void NativeOpenGLApp::OnPause() {}


void NativeOpenGLApp::OnResume() {}


void NativeOpenGLApp::OnSurfaceCreated(JNIEnv *env) {
  renderer.Initialize();
}


void NativeOpenGLApp::OnDrawFrame() {
  renderer.RenderFrame();
}


void NativeOpenGLApp::OnSurfaceChanged(int width, int height) {
  screen_width = width;
  screen_height = height;
}

}   // namespace ndk_opengl_app
