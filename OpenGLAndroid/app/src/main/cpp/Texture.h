//
// Created by mrinaal on 1/25/20.
//

#ifndef OPENGLANDROID_TEXTURE_H
#define OPENGLANDROID_TEXTURE_H

#include <jni.h>

// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif


namespace ndk_opengl_app {

class Texture {
 public:
  Texture();

  ~Texture();

  // Initializes the texture.
  //
  // After this is called the texture will be bound, replacing any previously
  // bound texture.
  //
  // @return True if initialization was successful.
  bool LoadTextureFromBitmap(JNIEnv *env, jobject bitmap);

  // Binds the texture, replacing any previously bound texture.
  void Bind() const;

 private:
  static GLuint active_id_counter;

  GLuint active_texture_id;
  GLuint texture_id;
};


}   // namespace ndk_opengl_app

#endif //OPENGLANDROID_TEXTURE_H
