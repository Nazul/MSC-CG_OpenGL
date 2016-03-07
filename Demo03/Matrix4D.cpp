//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 03 - Perspective and view
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "Matrix4D.h"
#include <math.h>


MATRIX4D operator*(MATRIX4D &A, MATRIX4D &B) {
  MATRIX4D R = Zero();

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      for (int k = 0; k < 4; k++)
        R.m[j][i] += A.m[j][k] * B.m[k][i];
  return R;
}

VECTOR4D operator*(MATRIX4D &A, VECTOR4D &V) {
  VECTOR4D R = { 0, 0, 0, 0 };

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      R.v[j] += A.m[j][i] * V.v[i];
  return R;
}

VECTOR4D operator*(VECTOR4D &V, MATRIX4D &A) {
  VECTOR4D R = { 0, 0, 0, 0 };

  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      R.v[j] += A.m[i][j] * V.v[i];
  return R;
}

VECTOR4D operator*(VECTOR4D &A, VECTOR4D &B) {
  VECTOR4D R = { A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w };

  return R;
}

VECTOR4D operator-(VECTOR4D &A, VECTOR4D &B) {
  VECTOR4D R = { A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w };

  return R;
}

VECTOR4D operator+(VECTOR4D &A, VECTOR4D &B) {
  VECTOR4D R = { A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w };

  return R;
}

VECTOR4D Cross3(VECTOR4D &A, VECTOR4D &B) {
  VECTOR4D R;

  R.x = A.y * B.z - A.z * B.y;
  R.y = B.x * A.z - B.z * A.x;
  R.z = A.x * B.y - A.y * B.x;
  R.w = 0;
  return R;
}

float Dot(VECTOR4D &A, VECTOR4D &B) {
  return  A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
}

float Magnity(VECTOR4D &A) {
  return sqrtf(Dot(A, A));
}

VECTOR4D Normalize(VECTOR4D &A) {
  float inv = 1.0f / Magnity(A);
  VECTOR4D R = { A.x * inv, A.y * inv, A.z * inv, A.w * inv };
  return R;
}

MATRIX4D Zero() {
  MATRIX4D Z;

  memset(&Z, 0, sizeof(MATRIX4D));
  return Z;
}

MATRIX4D Identity() {
  MATRIX4D I;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      I.m[j][i] = (i == j) ? 1.0f : 0.0f;

  return I;
}

MATRIX4D RotationX(float theta) {
  MATRIX4D R = Identity();

  R.m22 = R.m11 = cosf(theta);
  R.m12 = sinf(theta);
  R.m21 = -R.m12;
  return R;
}

MATRIX4D RotationY(float theta) {
  MATRIX4D R = Identity();

  R.m00 = R.m22 = cosf(theta);
  R.m20 = sinf(theta);
  R.m02 = -R.m20;
  return R;
}

MATRIX4D RotationZ(float theta) {
  MATRIX4D R = Identity();

  R.m11 = R.m00 = cosf(theta);
  R.m01 = sinf(theta);
  R.m10 = -R.m01;
  return R;
}

MATRIX4D Translation(float tx, float ty, float tz) {
  MATRIX4D T = Identity();

  T.m03 = tx;
  T.m13 = ty;
  T.m23 = tz;
  return T;
}

MATRIX4D Scaling(float sx, float sy, float sz) {
  MATRIX4D S = Identity();

  S.m00 = sx;
  S.m11 = sy;
  S.m22 = sz;
  return S;
}

MATRIX4D View(VECTOR4D &EyePos, VECTOR4D &Target, VECTOR4D &Up) {
  MATRIX4D View = Identity();
  VECTOR4D N = Normalize(EyePos - Target);
  VECTOR4D U = Normalize(Cross3(Up, N));
  VECTOR4D V = Cross3(N, U);

  View.m00 = U.x;
  View.m01 = U.y;
  View.m02 = U.z;
  View.m03 = -Dot(U, EyePos);
  View.m10 = V.x;
  View.m11 = V.y;
  View.m12 = V.z;
  View.m13 = -Dot(V, EyePos);
  View.m20 = N.x;
  View.m21 = N.y;
  View.m22 = N.z;
  View.m23 = -Dot(N, EyePos);

  return View;
}

MATRIX4D PerspectiveWidthHeightLH(int nWidth, int nHeight, float zNear, float zFar) {
  MATRIX4D P = Zero();

  P.m00 = 2.0f * zNear / nWidth;
  P.m11 = 2.0f * zNear / nHeight;
  P.m22 = -zFar / (zFar - zNear);
  P.m23 = -zNear * zFar / (zFar - zNear);
  P.m32 = -1.0f;

  return P;
}

MATRIX4D PerspectiveWidthHeightRH(float fWidth, float fHeight, float zNear, float zFar) {
  MATRIX4D P = Zero();

  P.m00 = zNear / fWidth;
  P.m11 = zNear / fHeight;
  P.m22 = -zFar / (zFar - zNear);
  P.m23 = -zNear * zFar / (zFar - zNear);
  P.m32 = -1.0f;

  return P;
}

MATRIX4D Transpose(MATRIX4D &M) {
  MATRIX4D R;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      R.m[i][j] = M.m[j][i];
  return R;
}

MATRIX4D FastInverse(MATRIX4D &M) {
  MATRIX4D R;
  VECTOR4D InvPos;

  R = Transpose(M);
  InvPos = R.vec[3];
  R.m03 = -Dot(R.vec[0], InvPos);
  R.m13 = -Dot(R.vec[1], InvPos);
  R.m23 = -Dot(R.vec[2], InvPos);
  R.vec[3].x = 0;
  R.vec[3].y = 0;
  R.vec[3].z = 0;

  return R;
}

// EOF
