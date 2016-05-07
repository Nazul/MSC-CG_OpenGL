#include "stdafx.h"
#include "Terrain.h"
#include <GL\GL.h>
#include <GL\GLU.h>

#define PI 3.141592f


float plane(float x, float y) {
  return 1.0f;
}

CTerrain::CTerrain(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, MATRIX4D* Vi, MATRIX4D* Pe, MATRIX4D* SA) {
  landMesh.BuildAnalyticSurface(nVx, nVy, x0, y0, dx, dy, plane);
  waterMesh.BuildAnalyticSurface(nVx, nVy, x0, y0, dx, dy, plane);
  V = Vi;
  P = Pe;
  S = SA;
  TL = Identity() * Translation(0.0f, 0.0f, -0.9f);
  TW = Identity() * Translation(0.0f, 0.0f, -1.0f);
  w = nVx;
  h = nVy;
}

void CTerrain::Draw() {
  VECTOR4D worldRayOrigin, worldRayDir;
  VECTOR4D modelRayOrigin, modelRayDir;
  MATRIX4D InvW, Land, Water;
  multimap<float, CMesh::INTERSECTIONINFO> faces;
  BOOL fill = FALSE;
  VECTOR4D g = { 0.0f, 1.0f, 0.0f, 1.0f };
  VECTOR4D b = { 0.0f, 0.0f, 1.0f, 0.75f };

  Land = *S * *P * *V * TL;
  Water = *S * *P * *V * TW;

  BuildRayFromPerspective(Land, x, y, worldRayOrigin, worldRayDir);
  Inverse(Identity(), InvW);
  modelRayOrigin = InvW * worldRayOrigin;
  modelRayDir = InvW * worldRayDir;
  fill = landMesh.RayCast(modelRayOrigin, modelRayDir, faces);

  // Water
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  waterMesh.SetColor(b, b, b, b);
  waterMesh.Draw(Water);
  glEnd();

  // Land (all)
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  landMesh.SetColor(g, g, g, g);
  landMesh.Draw(Land);
  glEnd();

  // Land (selection)
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  if (fill) {
    int center = faces.begin()->second.Face;
    int start = 0;

    if (center % 2 == 1)
      center--;
    switch (size) {
    case AreaSize::Small:
      start = center - h * 2;
      landMesh.Draw(Land, start < 0 ? 0 : start, 6);
      start = center - 2;
      landMesh.Draw(Land, start < 0 ? 0 : start, 6);
      start = center - 4 + h * 2;
      landMesh.Draw(Land, start < 0 ? 0 : start, 6);
      break;
    case AreaSize::Medium:
      start = center - h * 4;
      landMesh.Draw(Land, start < 0 ? 0 : start, 10);
      start = center - h * 2 - 2;
      landMesh.Draw(Land, start < 0 ? 0 : start, 10);
      start = center - 4;
      landMesh.Draw(Land, start < 0 ? 0 : start, 10);
      start = center + h * 2 - 6;
      landMesh.Draw(Land, start < 0 ? 0 : start, 10);
      start = center + h * 4 - 8;
      landMesh.Draw(Land, start < 0 ? 0 : start, 10);
      break;
    case AreaSize::Large:
      start = center - h * 6;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center - h * 4 - 2;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center - h * 2 - 4;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center - 6;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center + h * 2 - 8;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center + h * 4 - 10;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      start = center + h * 6 - 12;
      landMesh.Draw(Land, start < 0 ? 0 : start, 14);
      break;
    }
  }
  glEnd();
}

void CTerrain::MouseMove(float x, float y) {
  this->x = x;
  this->y = y;
}

void CTerrain::Press() {
  VECTOR4D worldRayOrigin, worldRayDir;
  VECTOR4D modelRayOrigin, modelRayDir;
  MATRIX4D InvW, Land, Water;
  multimap<float, CMesh::INTERSECTIONINFO> faces;
  BOOL fill = FALSE;

  Land = *S * *P * *V * TL;
  Water = *S * *P * *V * TW;

  BuildRayFromPerspective(Land, x, y, worldRayOrigin, worldRayDir);
  Inverse(Identity(), InvW);
  modelRayOrigin = InvW * worldRayOrigin;
  modelRayDir = InvW * worldRayDir;
  fill = landMesh.RayCast(modelRayOrigin, modelRayDir, faces);

  if (fill) {
    int center = faces.begin()->second.Face;
    int start = 0;

    if (center % 2 == 1)
      center--;
    switch (size) {
    case AreaSize::Small:
      start = center - h * 2;
      landMesh.Press(start < 0 ? 0 : start, 6);
      start = center - 2;
      landMesh.Press(start < 0 ? 0 : start, 6);
      start = center - 4 + h * 2;
      landMesh.Press(start < 0 ? 0 : start, 6);
      break;
    case AreaSize::Medium:
      start = center - h * 4;
      landMesh.Press(start < 0 ? 0 : start, 10);
      start = center - h * 2 - 2;
      landMesh.Press(start < 0 ? 0 : start, 10);
      start = center - 4;
      landMesh.Press(start < 0 ? 0 : start, 10);
      start = center + h * 2 - 6;
      landMesh.Press(start < 0 ? 0 : start, 10);
      start = center + h * 4 - 8;
      landMesh.Press(start < 0 ? 0 : start, 10);
      break;
    case AreaSize::Large:
      start = center - h * 6;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center - h * 4 - 2;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center - h * 2 - 4;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center - 6;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center + h * 2 - 8;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center + h * 4 - 10;
      landMesh.Press(start < 0 ? 0 : start, 14);
      start = center + h * 6 - 12;
      landMesh.Press(start < 0 ? 0 : start, 14);
      break;
    }
  }
}

void CTerrain::Raise() {
  VECTOR4D worldRayOrigin, worldRayDir;
  VECTOR4D modelRayOrigin, modelRayDir;
  MATRIX4D InvW, Land, Water;
  multimap<float, CMesh::INTERSECTIONINFO> faces;
  BOOL fill = FALSE;

  Land = *S * *P * *V * TL;
  Water = *S * *P * *V * TW;

  BuildRayFromPerspective(Land, x, y, worldRayOrigin, worldRayDir);
  Inverse(Identity(), InvW);
  modelRayOrigin = InvW * worldRayOrigin;
  modelRayDir = InvW * worldRayDir;
  fill = landMesh.RayCast(modelRayOrigin, modelRayDir, faces);

  if (fill) {
    int center = faces.begin()->second.Face;
    int start = 0;

    if (center % 2 == 1)
      center--;
    switch (size) {
    case AreaSize::Small:
      start = center - h * 2;
      landMesh.Raise(start < 0 ? 0 : start, 6);
      start = center - 2;
      landMesh.Raise(start < 0 ? 0 : start, 6);
      start = center - 4 + h * 2;
      landMesh.Raise(start < 0 ? 0 : start, 6);
      break;
    case AreaSize::Medium:
      start = center - h * 4;
      landMesh.Raise(start < 0 ? 0 : start, 10);
      start = center - h * 2 - 2;
      landMesh.Raise(start < 0 ? 0 : start, 10);
      start = center - 4;
      landMesh.Raise(start < 0 ? 0 : start, 10);
      start = center + h * 2 - 6;
      landMesh.Raise(start < 0 ? 0 : start, 10);
      start = center + h * 4 - 8;
      landMesh.Raise(start < 0 ? 0 : start, 10);
      break;
    case AreaSize::Large:
      start = center - h * 6;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center - h * 4 - 2;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center - h * 2 - 4;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center - 6;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center + h * 2 - 8;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center + h * 4 - 10;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      start = center + h * 6 - 12;
      landMesh.Raise(start < 0 ? 0 : start, 14);
      break;
    }
  }
}

void CTerrain::SetArea(AreaSize size) {
  this->size = size;
}

CTerrain::~CTerrain() {
}

// EOF
