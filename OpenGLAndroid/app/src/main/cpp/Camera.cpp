//
// Created by mrinaal on 2/16/20.
//

#include "Camera.h"


namespace ndk_opengl_app{

namespace {
#define DEFAULT_POS_X 0
#define DEFAULT_POS_Y 0
#define DEFAULT_POS_Z 3

#define DEFAULT_TARGET_X 0
#define DEFAULT_TARGET_Y 0
#define DEFAULT_TARGET_Z 0

#define DEFAULT_UP_X 0
#define DEFAULT_UP_Y 1
#define DEFAULT_UP_Z 0
}


Camera::Camera() {
  position[0] = initial_position[0] = DEFAULT_POS_X;
  position[1] = initial_position[1] = DEFAULT_POS_Y;
  position[2] = initial_position[2] = DEFAULT_POS_Z;

  target[0] = initial_target[0] = DEFAULT_TARGET_X;
  target[1] = initial_target[1] = DEFAULT_TARGET_Y;
  target[2] = initial_target[2] = DEFAULT_TARGET_Z;

  up[0] = initial_up[0] = DEFAULT_UP_X;
  up[1] = initial_up[1] = DEFAULT_UP_Y;
  up[2] = initial_up[2] = DEFAULT_UP_Z;
}


Camera::Camera(glmath::Vec3 _pos, glmath::Vec3 _target, glmath::Vec3 _up) {
  position[0] = initial_position[0] = _pos[0];
  position[1] = initial_position[1] = _pos[1];
  position[2] = initial_position[2] = _pos[2];

  target[0] = initial_target[0] = _target[0];
  target[1] = initial_target[1] = _target[1];
  target[2] = initial_target[2] = _target[2];

  up[0] = initial_up[0] = _up[0];
  up[1] = initial_up[1] = _up[1];
  up[2] = initial_up[2] = _up[2];
}


void Camera::ResetCamera() {
  position[0] = initial_position[0];
  position[1] = initial_position[1];
  position[2] = initial_position[2];

  target[0] = initial_target[0];
  target[1] = initial_target[1];
  target[2] = initial_target[2];

  up[0] = initial_up[0];
  up[1] = initial_up[1];
  up[2] = initial_up[2];
}


glmath::Matrix4x4 Camera::GetViewMatrix() {
  float radius = 10.0f;
  float angle = GetMonotonicTimeMilliSecs() * 0.002f;
  float camX   = sin(angle) * radius;
  float camZ   = cos(angle) * radius;

  position = glmath::Vec3(camX, 0.0f, camZ);
  target = glmath::Vec3(0.0f, 0.0f, 0.0f);
  up = glmath::Vec3(0.0f, 1.0f, 0.0f);

  glmath::Matrix4x4 viewMatrix;
  viewMatrix.SetToLookAt(position, target, up);

  return viewMatrix;
}


void Camera::Rotate(const float yaw, const float pitch) {
  LOGD("Camera::Rotate() Yaw: [%.5f]  Pitch: [%.5f]", yaw, pitch);
}


void Camera::Translate(const float diff_x, const float diff_y) {
  LOGD("Camera::Translate() X: [%.5f]  Y: [%.5f]", diff_x, diff_y);
}


void Camera::Zoom(const float scale_x, const float scale_y) {
  LOGD("Camera::Zoom() X: [%.5f]  Y: [%.5f]", scale_x, scale_y);
}

}   // namespace ndk_opengl_app