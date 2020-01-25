
#include <jni.h>
#include <memory>

#include "native_opengl_app.h"


namespace {

  inline jlong jptr(ndk_opengl_app::NativeOpenGLApp* native_app) {
    return reinterpret_cast<intptr_t>(native_app);
  }

  inline ndk_opengl_app::NativeOpenGLApp* native_ptr(jlong ptr) {
    return reinterpret_cast<ndk_opengl_app::NativeOpenGLApp*>(ptr);
  }

  JavaVM* javaVm;

}  // anonymous namespace


extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  javaVm = vm;
  return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnCreate(
        JNIEnv* env,
        jobject obj ) {
  return jptr(new ndk_opengl_app::NativeOpenGLApp(javaVm));
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnDestroy(
        JNIEnv* env,
        jobject obj,
        jlong native_app ) {
  delete native_ptr(native_app);
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnPause(
        JNIEnv* env,
        jobject obj,
        jlong native_app ) {
  native_ptr(native_app)->OnPause();
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnResume(
        JNIEnv* env,
        jobject obj,
        jlong native_app ) {
  native_ptr(native_app)->OnResume();
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnSurfaceCreated(
        JNIEnv* env,
        jobject obj,
        jlong native_app ) {
  native_ptr(native_app)->OnSurfaceCreated(env);
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnSurfaceChanged(
        JNIEnv* env,
        jobject obj,
        jlong native_app,
        jint width,
        jint height ) {
  native_ptr(native_app)->OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeOnDrawFrame(
        JNIEnv* env,
        jobject obj,
        jlong native_app ) {
  native_ptr(native_app)->OnDrawFrame();
}

JNIEXPORT void JNICALL
Java_com_example_openglandroid_MainActivity_nativeLoadTextureFromBitmap(
        JNIEnv* env,
        jobject obj,
        jlong native_app,
        jobject bitmap ) {
  native_ptr(native_app)->LoadTextureFromBitmap(env, bitmap);
}

}   // extern "C"