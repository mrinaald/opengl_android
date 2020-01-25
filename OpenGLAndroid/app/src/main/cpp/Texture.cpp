//
// Created by mrinaal on 1/25/20.
//

#include "Texture.h"

namespace ndk_opengl_app {


GLuint Texture::active_id_counter = 0;


Texture::Texture()
        : active_texture_id(0),
          texture_id(0) {}


Texture::~Texture() = default;


bool Texture::LoadTextureFromBitmap(JNIEnv *env, jobject bitmap, int new_active_texture_id) {
  if (new_active_texture_id < active_id_counter)
    active_texture_id = static_cast<GLuint>(new_active_texture_id);
  else
    active_texture_id = active_id_counter++;
  glGenTextures(1, &texture_id);

  Bind();

  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  jclass gl_utils_class = env->FindClass("android/opengl/GLUtils");

  jmethodID tex_image_2d_method = env->GetStaticMethodID(
          gl_utils_class, "texImage2D", "(IILandroid/graphics/Bitmap;I)V");

  env->CallStaticVoidMethod(gl_utils_class, tex_image_2d_method, GL_TEXTURE_2D, 0, bitmap, 0);

  glGenerateMipmap(GL_TEXTURE_2D);

  return true;
}


void Texture::Bind() const {
  glActiveTexture(GL_TEXTURE0 + active_texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

}   // namespace ndk_opengl_app
