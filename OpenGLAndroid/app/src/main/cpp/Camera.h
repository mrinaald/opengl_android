//
// Created by mrinaal on 2/16/20.
//

#ifndef OPENGLANDROID_CAMERA_H
#define OPENGLANDROID_CAMERA_H

#include "GLMath.h"
#include "utils.h"


namespace ndk_opengl_app {

class Camera {
 public:
  Camera();

  Camera(glmath::Vec3 _pos, glmath::Vec3 _target, glmath::Vec3 _up);

  void ResetCamera();

  void Rotate(const float yaw, const float pitch);

  void Translate(const float diff_x, const float diff_y);

  void Zoom(const float scale_x, const float scale_y);

  glmath::Matrix4x4 GetViewMatrix();

 private:

  glmath::Vec3 position;
  glmath::Vec3 target;
  glmath::Vec3 up;

  glmath::Vec3 initial_position;
  glmath::Vec3 initial_target;
  glmath::Vec3 initial_up;
};

}   // ndk_opengl_app

#endif //OPENGLANDROID_CAMERA_H
