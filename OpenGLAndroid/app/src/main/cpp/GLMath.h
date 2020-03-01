//
// Created by mrinaal on 1/25/20.
//

#ifndef OPENGLANDROID_GLMATH_H
#define OPENGLANDROID_GLMATH_H

#include <array>
#include <cmath>

namespace glmath {

float DegToRad(const float deg);

class Vec3 {
 public:
  Vec3();

  Vec3(float x, float y, float z);

  // Dot product with another Vec3
  float Dot(const Vec3& vec3) const;

  // Cross product with another Vec3
  Vec3 Cross(const Vec3& vec3) const;

  // Vector Norm
  float Norm() const;

  // Normalize Vector
  void Normalize();

  float AngleWithVector(const Vec3& vec3) const;

  // For easier getting
  float operator [](const int i) const { return vec[i]; }

  // For easier setting
  float & operator [](const int i) { return vec[i]; }

  Vec3 operator+(const Vec3& rhs) const;

  Vec3 operator-(const Vec3& rhs) const;

  const float* GetData() const;


 private:
  std::array<float, 3> vec;
};    // class Vec3


class Vec4 {
 public:
  Vec4();

  Vec4(float x, float y, float z, float w);

  // Dot product with another Vec4
  float Dot(const Vec4& vec4) const;

  // Vector Norm
  float Norm() const;

  // Normalize Vector
  void Normalize();

  float AngleWithVector(const Vec4& vec4) const;

  // For easier getting
  float operator [](const int i) const { return vec[i]; }

  // For easier setting
  float & operator [](const int i) { return vec[i]; }

  Vec4 operator+(const Vec4& rhs) const;

  Vec4 operator-(const Vec4& rhs) const;

  const float* GetData() const;

 private:
  std::array<float, 4> vec;
};    // class Vec4


/**
 * OpenGL developers often use an internal matrix layout called column-major ordering
 * This Matrix4x4 class maintains the matrix in transposed fashion. Thus, there is no
 * need to transpose this matrix while passing it to OpenGL
 */
class Matrix4x4 {
 public:
  // Identity Matrix Constructor
  Matrix4x4();

  // Construct Matrix from GLArray
  Matrix4x4(float *gl_vec);

  // Construct Rotation Matrix from Quaternions
  void FromQuaternion(float q[4]);

  // Multiplies two matrices.
  Matrix4x4 operator*(const Matrix4x4& right);

  // Multiplies a matrix with a vector.
  Vec4 operator*(const Vec4& vec);

  // Converts a matrix to an array of floats suitable for passing to OpenGL.
  std::array<float, 16> ToGlArray() const;

  // Translate Matrix
  void Translate(const Vec3& vec);

  // Scale Matrix
  void Scale(const Vec3& vec);

  // Rotate Matrix
  void Rotate(const float rad_angle, const Vec3& vec);

  // Create Orthographic Projection Matrix
  void SetToOrthographic(float l, float r, float b, float t, float n, float f);

  // Create Perspective Projection Matrix
  void SetToPerspective(float fovy, float aspect_ratio, float near, float far);

  // Set to Identity Matrix
  void SetToIdentity();

  // Set to LookAt Matrix
  void SetToLookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

 private:
  float mat[4][4];
};    // class Matrix4x4

}   // namespace glmath

#endif //OPENGLANDROID_GLMATH_H
