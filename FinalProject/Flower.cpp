//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "Flower.h"
#include <GL\GL.h>
#include <GL\GLU.h>

#define PI 3.141592f

VECTOR4D sphere(float u, float v) {
  VECTOR4D V;
  float r = 0.4f;

  V.x = r * (cosf(2.0f * PI * u) * sinf(PI * v));
  V.y = r * (sinf(2.0f * PI * u) * sinf(PI * v));
  V.z = r * (cosf(PI * v));
  V.w = 1.0f;

  return V;
}

VECTOR4D dini(float u, float v) {
  VECTOR4D V;
  float a = 1.0f;
  float b = 0.2f;

  V.x = 0.5f * (a * cosf(u) * sinf(v));
  V.y = 0.5f * (a * sinf(u) * sinf(v));
  V.z = 0.5f * (a * (cosf(v) + logf(tanf(v * 0.5f))) + b * u);
  V.w = 1.0f;

  return V;
}

CFlower::CFlower(unsigned long nVx, unsigned long nVy, float x0, float y0, float du, float dv, MATRIX4D * Vi, MATRIX4D * Pe, MATRIX4D * SA) {
  VECTOR4D c1 = { 1.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D c2 = { 0.9f, 0.0f, 0.0f, 1.0f };
  VECTOR4D c3 = { 0.8f, 0.0f, 0.0f, 1.0f };
  VECTOR4D c4 = { 0.7f, 0.0f, 0.0f, 1.0f };

  mesh.BuildParametricSurface(nVx, nVy, x0, y0, du, dv, dini);
  mesh.SetColor(c1, c2, c3, c4);
  V = Vi;
  P = Pe;
  S = SA;
}

void CFlower::Draw() {
  if (Ts.size() > 0) {
    for each (MATRIX4D T in Ts) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glBegin(GL_TRIANGLES);
      mesh.Draw(*S * *P * *V * T);
      glEnd();
    }
  }
}

void CFlower::Add(VECTOR4D V) {
  MATRIX4D T;
  //T = Scaling(0.05f, 0.05f, 0.05f) * Translation((float)rand() / RAND_MAX * 18, (float)rand() / RAND_MAX * 18, 4.0f);
  T = Identity() * Scaling(0.05f, 0.05f, 0.05f) * Translation(V.x * 20, V.y * 20, V.z + 2);

  Ts.push_back(T);
}

void CFlower::Delete() {
}

CFlower::~CFlower() {
}

// EOF
