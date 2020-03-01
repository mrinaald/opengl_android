//
// Created by mrinaal on 1/19/20.
//

#ifndef OPENGLANDROID_UTILS_H
#define OPENGLANDROID_UTILS_H

#include <android/log.h>
#include <jni.h>


// Logging
#define LOG_TAG "NativeOpenGLApp"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


namespace ndk_opengl_app {
 /**
  * Gets monotonic time in seconds.
  *
  * @return Monotonic time in seconds
  */
long GetMonotonicTimeMilliSecs();
}


#endif //OPENGLANDROID_UTILS_H
