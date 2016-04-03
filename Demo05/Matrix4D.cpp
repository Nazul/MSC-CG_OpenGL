//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 05 - Mouse & Raycast
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "Matrix4D.h"
#include <math.h>

#define PI 3.141592f
#define EPSILON 0.0001f


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

VECTOR4D Lerp(VECTOR4D & A, VECTOR4D & B, float u) {
  VECTOR4D U = {u, u, u, u};
  return A + U * (B - A);
}

bool PtInTriangle(VECTOR4D &V0, VECTOR4D &V1, VECTOR4D &V2, VECTOR4D &P) {
  float alpha, beta, gamma;
  VECTOR4D I = V0 - P, J = V1 - P, K = V2 - P;
  float mI = Magnity(I), mJ = Magnity(J), mK = Magnity(K);

  alpha = acosf(Dot(I, J) / (mI * mJ));
  beta = acosf(Dot(J, K) / (mJ * mK));
  gamma = acosf(Dot(K, I) / (mK * mI));

  return fabsf((2 * PI - (alpha + beta + gamma))) < EPSILON;
}

bool PtInTriangleBarycentric(VECTOR4D &V0, VECTOR4D &V1, VECTOR4D &V2, VECTOR4D &P, float &w0, float &w1, float &w2) {
  float A, B, C;
  float D, E, F;
  float invDet;

  A = Dot( P - V0, V1 - V0);
  B = Dot(V1 - V0, V1 - V0);
  C = Dot(V2 - V0, V1 - V0);
  D = Dot( P - V0, V2 - V0);
  E = Dot(V1 - V0, V2 - V0);
  F = Dot(V2 - V0, V2 - V0);

  invDet = 1.0f / (B * F - E * C);
  w1 = (F * A - C * D) * invDet;
  w2 = (B * D - E * A) * invDet;
  w0 = 1 - (w1 + w2);

  return (w1 + w2) < 1 && w1 >= 0 && w2 >= 0;
}

//Matrix Inverse
float Inverse(MATRIX4D& M, MATRIX4D& R) {
  double inv[16], det;
  int i;

  inv[0] = M.v[5] * M.v[10] * M.v[15] -
    M.v[5] * M.v[11] * M.v[14] -
    M.v[9] * M.v[6] * M.v[15] +
    M.v[9] * M.v[7] * M.v[14] +
    M.v[13] * M.v[6] * M.v[11] -
    M.v[13] * M.v[7] * M.v[10];

  inv[4] = -M.v[4] * M.v[10] * M.v[15] +
    M.v[4] * M.v[11] * M.v[14] +
    M.v[8] * M.v[6] * M.v[15] -
    M.v[8] * M.v[7] * M.v[14] -
    M.v[12] * M.v[6] * M.v[11] +
    M.v[12] * M.v[7] * M.v[10];

  inv[8] = M.v[4] * M.v[9] * M.v[15] -
    M.v[4] * M.v[11] * M.v[13] -
    M.v[8] * M.v[5] * M.v[15] +
    M.v[8] * M.v[7] * M.v[13] +
    M.v[12] * M.v[5] * M.v[11] -
    M.v[12] * M.v[7] * M.v[9];

  inv[12] = -M.v[4] * M.v[9] * M.v[14] +
    M.v[4] * M.v[10] * M.v[13] +
    M.v[8] * M.v[5] * M.v[14] -
    M.v[8] * M.v[6] * M.v[13] -
    M.v[12] * M.v[5] * M.v[10] +
    M.v[12] * M.v[6] * M.v[9];

  inv[1] = -M.v[1] * M.v[10] * M.v[15] +
    M.v[1] * M.v[11] * M.v[14] +
    M.v[9] * M.v[2] * M.v[15] -
    M.v[9] * M.v[3] * M.v[14] -
    M.v[13] * M.v[2] * M.v[11] +
    M.v[13] * M.v[3] * M.v[10];

  inv[5] = M.v[0] * M.v[10] * M.v[15] -
    M.v[0] * M.v[11] * M.v[14] -
    M.v[8] * M.v[2] * M.v[15] +
    M.v[8] * M.v[3] * M.v[14] +
    M.v[12] * M.v[2] * M.v[11] -
    M.v[12] * M.v[3] * M.v[10];

  inv[9] = -M.v[0] * M.v[9] * M.v[15] +
    M.v[0] * M.v[11] * M.v[13] +
    M.v[8] * M.v[1] * M.v[15] -
    M.v[8] * M.v[3] * M.v[13] -
    M.v[12] * M.v[1] * M.v[11] +
    M.v[12] * M.v[3] * M.v[9];

  inv[13] = M.v[0] * M.v[9] * M.v[14] -
    M.v[0] * M.v[10] * M.v[13] -
    M.v[8] * M.v[1] * M.v[14] +
    M.v[8] * M.v[2] * M.v[13] +
    M.v[12] * M.v[1] * M.v[10] -
    M.v[12] * M.v[2] * M.v[9];

  inv[2] = M.v[1] * M.v[6] * M.v[15] -
    M.v[1] * M.v[7] * M.v[14] -
    M.v[5] * M.v[2] * M.v[15] +
    M.v[5] * M.v[3] * M.v[14] +
    M.v[13] * M.v[2] * M.v[7] -
    M.v[13] * M.v[3] * M.v[6];

  inv[6] = -M.v[0] * M.v[6] * M.v[15] +
    M.v[0] * M.v[7] * M.v[14] +
    M.v[4] * M.v[2] * M.v[15] -
    M.v[4] * M.v[3] * M.v[14] -
    M.v[12] * M.v[2] * M.v[7] +
    M.v[12] * M.v[3] * M.v[6];

  inv[10] = M.v[0] * M.v[5] * M.v[15] -
    M.v[0] * M.v[7] * M.v[13] -
    M.v[4] * M.v[1] * M.v[15] +
    M.v[4] * M.v[3] * M.v[13] +
    M.v[12] * M.v[1] * M.v[7] -
    M.v[12] * M.v[3] * M.v[5];

  inv[14] = -M.v[0] * M.v[5] * M.v[14] +
    M.v[0] * M.v[6] * M.v[13] +
    M.v[4] * M.v[1] * M.v[14] -
    M.v[4] * M.v[2] * M.v[13] -
    M.v[12] * M.v[1] * M.v[6] +
    M.v[12] * M.v[2] * M.v[5];

  inv[3] = -M.v[1] * M.v[6] * M.v[11] +
    M.v[1] * M.v[7] * M.v[10] +
    M.v[5] * M.v[2] * M.v[11] -
    M.v[5] * M.v[3] * M.v[10] -
    M.v[9] * M.v[2] * M.v[7] +
    M.v[9] * M.v[3] * M.v[6];

  inv[7] = M.v[0] * M.v[6] * M.v[11] -
    M.v[0] * M.v[7] * M.v[10] -
    M.v[4] * M.v[2] * M.v[11] +
    M.v[4] * M.v[3] * M.v[10] +
    M.v[8] * M.v[2] * M.v[7] -
    M.v[8] * M.v[3] * M.v[6];

  inv[11] = -M.v[0] * M.v[5] * M.v[11] +
    M.v[0] * M.v[7] * M.v[9] +
    M.v[4] * M.v[1] * M.v[11] -
    M.v[4] * M.v[3] * M.v[9] -
    M.v[8] * M.v[1] * M.v[7] +
    M.v[8] * M.v[3] * M.v[5];

  inv[15] = M.v[0] * M.v[5] * M.v[10] -
    M.v[0] * M.v[6] * M.v[9] -
    M.v[4] * M.v[1] * M.v[10] +
    M.v[4] * M.v[2] * M.v[9] +
    M.v[8] * M.v[1] * M.v[6] -
    M.v[8] * M.v[2] * M.v[5];

  det = M.v[0] * inv[0] + M.v[1] * inv[4] + M.v[2] * inv[8] + M.v[3] * inv[12];

  if (fabs(det) < 0.0001)
    return 0.0f;

  double invdet = 1.0 / det;

  for (i = 0; i < 16; i++)
    R.v[i] = (float)(inv[i] * invdet);

  return (float)det;
}

void PlaneIntersect(VECTOR4D &RayOrigin, VECTOR4D &RayDir, VECTOR4D &Plane, float &n, float &d) {
  n = -Dot(Plane, RayOrigin);
  d = Dot(Plane, RayDir);
}

bool RayCastOnTriangle(VECTOR4D &V0, VECTOR4D &V1, VECTOR4D &V2, VECTOR4D &RayOrigin, VECTOR4D &RayDir, VECTOR4D &Intersection) {
  VECTOR4D Plane, Offset;
  float n, d, u, w0, w1, w2;
  
  Plane = Cross3((V1 - V0), (V2, V0));
  Plane = Normalize(Plane);
  Plane.w = 0.0f;

  PlaneIntersect(RayOrigin, RayDir, Plane, n, d);
  if (fabs(d) < 0.0001f)
    return false;

  u = n / d;
  if (u < 0.0f)
    return false;
  Offset = {RayDir.x * u, RayDir.y * u, RayDir.z * u, 0.0f};
  Intersection = RayOrigin + Offset;

  return PtInTriangleBarycentric(V0, V1, V2, Intersection, w0, w1, w2);
}

void BuildRayFromPerspective(MATRIX4D &PV, float x, float y, VECTOR4D &RayOrigin, VECTOR4D &RayDir) {
  MATRIX4D InvPV;
  VECTOR4D ScreenOrigin = {0.0f, 0.0f, 1.0f, 0.0f};
  VECTOR4D ScreenCursor = { x, y, 0.0f, 1.0f };

  Inverse(PV, InvPV); // Clipping space to world space

  RayOrigin = InvPV * ScreenOrigin;
  RayOrigin.x /= RayOrigin.w;
  RayOrigin.y /= RayOrigin.w;
  RayOrigin.z /= RayOrigin.w;
  RayOrigin.w = 1.0f;

  RayDir = InvPV * ScreenCursor;
  RayDir.x /= RayDir.w;
  RayDir.y /= RayDir.w;
  RayDir.z /= RayDir.w;
  RayDir.w = 1.0f;

  RayDir = Normalize(RayDir - RayOrigin);
}

// EOF
