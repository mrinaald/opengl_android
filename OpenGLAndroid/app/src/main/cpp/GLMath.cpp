//
// Created by mrinaal on 1/25/20.
//

#include "GLMath.h"

namespace glmath {

float DegToRad(const float deg) {
  return (float) ((deg * M_PI) / 180);
}

/* ** Vec3 Defintions ** */

Vec3::Vec3() : Vec3(0, 0, 0) {}

Vec3::Vec3(float x, float y, float z)
        : vec{x, y, z} {}

float Vec3::Dot(const glmath::Vec3 &vec3) const {
  return (vec[0]*vec3[0]) + (vec[1]*vec3[1]) + (vec[2]*vec3[2]);
}

float Vec3::Norm() const {
  return std::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]));
}

float Vec3::AngleWithVector(const glmath::Vec3 &vec3) const {
  float norm_dot = Dot(vec3) / (Norm() * vec3.Norm());
  return std::acos(std::max(-1.0f, std::min(1.0f, norm_dot)));
}

/* ** End of Vec3 Defintions ** */


/* ** Vec4 Defintions ** */

Vec4::Vec4() : Vec4(0, 0, 0, 0) {}

Vec4::Vec4(float x, float y, float z, float w)
        : vec{x, y, z, w} {}

float Vec4::Dot(const glmath::Vec4 &vec4) const {
  return (vec[0]*vec4[0]) + (vec[1]*vec4[1]) + (vec[2]*vec4[2]) + (vec[3]*vec4[3]);
}

float Vec4::Norm() const {
  return std::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]));
}

float Vec4::AngleWithVector(const glmath::Vec4 &vec4) const {
  float norm_dot = Dot(vec4) / (Norm() * vec4.Norm());
  return std::acos(std::max(-1.0f, std::min(1.0f, norm_dot)));
}

/* ** End of Vec4 Defintions ** */


/* ** Matrix4x4 Defintions ** */

// Identity Matrix Constructor
Matrix4x4::Matrix4x4() {
  mat[0][0] = 1.0f;
  mat[0][1] = 0.0f;
  mat[0][2] = 0.0f;
  mat[0][3] = 0.0f;

  mat[1][0] = 0.0f;
  mat[1][1] = 1.0f;
  mat[1][2] = 0.0f;
  mat[1][3] = 0.0f;

  mat[2][0] = 0.0f;
  mat[2][1] = 0.0f;
  mat[2][2] = 1.0f;
  mat[2][3] = 0.0f;

  mat[3][0] = 0.0f;
  mat[3][1] = 0.0f;
  mat[3][2] = 0.0f;
  mat[3][3] = 1.0f;
}

// Construct Matrix from GLArray
Matrix4x4::Matrix4x4(float *gl_vec) {
  memcpy(mat, gl_vec, 16 * sizeof(float));
}

// Construct Rotation Matrix from Quaternions
// NOTE: This function WILL REPLACE the contents of the matrix
void Matrix4x4::FromQuaternion(float q[4]) {
  const float x = q[0];
  const float y = q[1];
  const float z = q[2];
  const float w = q[3];

  // Based on ion::math::RotationMatrix3x3
  const float xx = 2 * x * x;
  const float yy = 2 * y * y;
  const float zz = 2 * z * z;

  const float xy = 2 * x * y;
  const float xz = 2 * x * z;
  const float yz = 2 * y * z;

  const float xw = 2 * x * w;
  const float yw = 2 * y * w;
  const float zw = 2 * z * w;

  mat[0][0] = 1 - yy - zz;
  mat[0][1] = xy + zw;
  mat[0][2] = xz - yw;
  mat[0][3] = 0;

  mat[1][0] = xy - zw;
  mat[1][1] = 1 - xx - zz;
  mat[1][2] = yz + xw;
  mat[1][3] = 0;

  mat[2][0] = xz + yw;
  mat[2][1] = yz - xw;
  mat[2][2] = 1 - xx - yy;
  mat[2][3] = 0;

  mat[3][0] = 0;
  mat[3][1] = 0;
  mat[3][2] = 0;
  mat[3][3] = 1;
}

Matrix4x4 Matrix4x4::operator*(const glmath::Matrix4x4 &right) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.mat[i][j] = 0.0f;
      for (int k = 0; k < 4; ++k) {
        result.mat[i][j] += this->mat[k][j] * right.mat[i][k];
      }
    }
  }
  return result;
}

Vec4 Matrix4x4::operator*(const glmath::Vec4 &vec) {
  Vec4 result{};
  for (int i = 0; i < 4; ++i) {
    result[i] = 0;
    for (int k = 0; k < 4; ++k) {
      result[i] += this->mat[k][i] * vec[k];
    }
  }
  return result;
}

std::array<float, 16> Matrix4x4::ToGlArray() {
  std::array<float, 16> result{};
  memcpy(&result[0], mat, 16 * sizeof(float));
  return result;
}

void Matrix4x4::Translate(const glmath::Vec3 &vec) {
  Matrix4x4 tmat;
  tmat.mat[3][0] = vec[0];
  tmat.mat[3][1] = vec[1];
  tmat.mat[3][2] = vec[2];
  tmat.mat[3][3] = 1.0f;

  *this = tmat * (*this);
}

void Matrix4x4::Scale(const glmath::Vec3 &vec) {
  Matrix4x4 smat;
  smat.mat[0][0] = vec[0];
  smat.mat[1][1] = vec[1];
  smat.mat[2][2] = vec[2];
  smat.mat[3][3] = 1.0f;

  *this = smat * (*this);
}

void Matrix4x4::Rotate(const float rad_angle, const glmath::Vec3 &vec) {
  float sin_theta = std::sin(rad_angle / 2);
  float cos_theta = std::cos(rad_angle / 2);

  float norm = std::sqrt((vec[0]*vec[0]) + (vec[1]*vec[1]) + (vec[2]*vec[2]));

  float quaternion[4];
  quaternion[0] = (vec[0] / norm) * sin_theta;
  quaternion[1] = (vec[1] / norm) * sin_theta;
  quaternion[2] = (vec[2] / norm) * sin_theta;
  quaternion[3] = cos_theta;

  Matrix4x4 rmat;
  rmat.FromQuaternion(quaternion);

  *this = rmat * (*this);
}

// NOTE: This function WILL REPLACE the contents of the matrix
void Matrix4x4::SetToOrthographic(float left, float right,
        float bottom, float top,
        float near, float far) {

  this->SetToIdentity();

  this->mat[0][0] = (2) / (right - left);
  this->mat[1][1] = (2) / (top - bottom);
  this->mat[2][2] = - (2) / (far - near);
  this->mat[3][0] = - (right + left) / (right - left);
  this->mat[3][1] = - (top + bottom) / (top - bottom);
  this->mat[3][2] = - (far + near) / (far - near);
}

// NOTE: This function WILL REPLACE the contents of the matrix
void Matrix4x4::SetToPerspective(float fovy, float aspect_ratio, float near, float far) {
  if (std::abs(aspect_ratio - std::numeric_limits<float>::epsilon()) <= 0) {
    // aspect ratio is very small.
    abort();
  }

  const float tan_half_fovy = std::tan( fovy / 2.0f);
  this->SetToIdentity();

  this->mat[0][0] = (1) / (aspect_ratio * tan_half_fovy);
  this->mat[1][1] = (1) / (tan_half_fovy);
  this->mat[2][2] = - (far + near) / (far - near);
  this->mat[2][3] = - (1);
  this->mat[3][2] = - (2 * far * near) / (far - near);
}

// NOTE: This function WILL REPLACE the contents of the matrix
void Matrix4x4::SetToIdentity() {
  this->mat[0][0] = 1.0f;
  this->mat[0][1] = 0.0f;
  this->mat[0][2] = 0.0f;
  this->mat[0][3] = 0.0f;

  this->mat[1][0] = 0.0f;
  this->mat[1][1] = 1.0f;
  this->mat[1][2] = 0.0f;
  this->mat[1][3] = 0.0f;

  this->mat[2][0] = 0.0f;
  this->mat[2][1] = 0.0f;
  this->mat[2][2] = 1.0f;
  this->mat[2][3] = 0.0f;

  this->mat[3][0] = 0.0f;
  this->mat[3][1] = 0.0f;
  this->mat[3][2] = 0.0f;
  this->mat[3][3] = 1.0f;
}

/* ** End of Matrix4x4 Defintions ** */

}   // namespace glmath
