//
// Created by mrinaal on 1/19/20.
//

#ifndef OPENGLANDROID_NATIVE_OPENGL_APP_H
#define OPENGLANDROID_NATIVE_OPENGL_APP_H

#include <jni.h>

#include <memory>
#include <thread>
#include <vector>

// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif

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


   private:
    int screen_width;
    int screen_height;

    GLuint shader_program;
    GLuint vertex_array_obj;
    GLuint vertex_buffer_obj;
  };
}

#endif //OPENGLANDROID_NATIVE_OPENGL_APP_H
