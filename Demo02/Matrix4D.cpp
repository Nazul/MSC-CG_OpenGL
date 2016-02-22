#include "stdafx.h"
#include "Matrix4D.h"
#include <math.h>


MATRIX4D operator*(MATRIX4D & A, MATRIX4D & B) {
  MATRIX4D R = Zero();

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      for (int k = 0; k < 4; k++)
        R.m[j][i] += A.m[j][k] * B.m[k][i];

  return R;
}

VECTOR4D operator*(MATRIX4D & A, VECTOR4D & V) {
  VECTOR4D R = {0, 0, 0, 0};

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      R.v[j] += A.m[j][i] * V.v[i];

    return R;
}

VECTOR4D operator*(VECTOR4D & V, MATRIX4D & A) {
  VECTOR4D R = { 0, 0, 0, 0 };

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      R.v[j] += A.m[i][j] * V.v[i];

  return R;
}

VECTOR4D operator*(VECTOR4D & A, VECTOR4D & B) {
  VECTOR4D R = { A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w };
  return R;
}

VECTOR4D Cross3(VECTOR4D & A, VECTOR4D & B) {
  VECTOR4D R;

  R.x = A.y * B.z - A.z * B.y;
  R.y = B.x * A.z - B.z * A.x;
  R.z = A.x * B.y - A.y * B.x;
  R.w = 0;

  return R;
}

float Dot(VECTOR4D & A, VECTOR4D & B) {
  return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
}

float Magnity(VECTOR4D & A) {
  return sqrtf(Dot(A, A));
}

VECTOR4D Normalize(VECTOR4D & A) {
  float inv = 1.0f / Magnity(A);
  VECTOR4D R = {A.x * inv, A.y * inv, A.z * inv, A.w * inv};

  return R;
}

MATRIX4D Zero() {
  MATRIX4D R;

  for (int i = 0; i < 16; i++)
    R.v[i] = 0.0f;
  return R;
}

MATRIX4D Identity() {
  MATRIX4D R;

  R.vec[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
  R.vec[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
  R.vec[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
  R.vec[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
  return R;
}

MATRIX4D RotationX(float theta) {
  MATRIX4D R = Zero();

  R.m00 = 1.0f;
  R.m01 = 0.0f;
  R.m02 = 0.0f;
  R.m03 = 0.0f;
  R.m10 = 0.0f;
  R.m11 = cosf(theta * PI / 180.0f);
  R.m12 = -sinf(theta * PI / 180.0f);
  R.m13 = 0.0f;
  R.m20 = 0.0f;
  R.m21 = sinf(theta * PI / 180.0f);
  R.m22 = cosf(theta * PI / 180.0f);
  R.m23 = 0.0f;
  R.m30 = 0.0f;
  R.m31 = 0.0f;
  R.m32 = 0.0f;
  R.m33 = 1.0f;

  return R;
}

MATRIX4D RotationY(float theta) {
  MATRIX4D R = Zero();

  R.m00 = cosf(theta * PI / 180.0f);
  R.m01 = 0.0f;
  R.m02 = sinf(theta * PI / 180.0f);
  R.m03 = 0.0f;
  R.m10 = 0.0f;
  R.m11 = 1.0f;
  R.m12 = 0.0f;
  R.m13 = 0.0f;
  R.m20 = -sinf(theta * PI / 180.0f);
  R.m21 = 0.0f;
  R.m22 = cosf(theta * PI / 180.0f);
  R.m23 = 0.0f;
  R.m30 = 0.0f;
  R.m31 = 0.0f;
  R.m32 = 0.0f;
  R.m33 = 1.0f;

  return R;
}

MATRIX4D RotationZ(float theta) {
  MATRIX4D R = Zero();

  R.m00 = cosf(theta * PI / 180.0f);
  R.m01 = -sinf(theta * PI / 180.0f);
  R.m02 = 0.0f;
  R.m03 = 0.0f;
  R.m10 = sinf(theta * PI / 180.0f);
  R.m11 = cosf(theta * PI / 180.0f);
  R.m12 = 0.0f;
  R.m13 = 0.0f;
  R.m20 = 0.0f;
  R.m21 = 0.0f;
  R.m22 = 0.0f;
  R.m23 = 0.0f;
  R.m30 = 0.0f;
  R.m31 = 0.0f;
  R.m32 = 0.0f;
  R.m33 = 1.0f;

  return R;
}

MATRIX4D Translation(float dx, float dy, float dz) {
  MATRIX4D R = Identity();

  R.vec[0].w = dx;
  R.vec[1].w = dy;
  R.vec[2].w = dz;
  return R;
}

MATRIX4D Scaling(float dx, float dy, float dz) {
  return MATRIX4D();
}

MATRIX4D View(VECTOR4D & EyePos, VECTOR4D & Target, VECTOR4D & Up) {
  return MATRIX4D();
}
