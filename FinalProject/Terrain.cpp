//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "Terrain.h"
#include <GL\GL.h>
#include <GL\GLU.h>

#define PI 3.141592f

VECTOR4D SELECTED_LAND_COLOR = { 0.8f, 0.8f, 0.0f, 0.9f };
VECTOR4D WIREFRAME_LAND_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };
VECTOR4D LAND_COLOR = { 0.0f, 0.5f, 0.0f, 0.8f };
VECTOR4D WATER_COLOR = { 0.0f, 0.0f, 1.0f, 0.35f };
VECTOR4D SELECTED_CENTER_COLOR = { 1.0f, 0.0f, 0.0f, 0.9f };


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
  MATRIX4D Land, LandWF, Water;

  Land = *S * *P * *V * TL;
  LandWF = *S * *P * *V * TL * Translation(0.0f, 0.0f, 0.002f);
  Water = *S * *P * *V * TW;

  // Land (selection)
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  if (faceSelected) {
    int center = selectedFace;
    int start = 0;

    if (center % 2 == 1)
      center--;
    landMesh.SetColor(SELECTED_LAND_COLOR, SELECTED_LAND_COLOR, SELECTED_LAND_COLOR, SELECTED_LAND_COLOR);
    switch (size) {
    case AreaSize::Single:
    {

      landMesh.SetColor(SELECTED_CENTER_COLOR, SELECTED_CENTER_COLOR, SELECTED_CENTER_COLOR, SELECTED_CENTER_COLOR);
      landMesh.Draw(Land, center, 2);
    }
    break;
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

  // Land (all - wireframe)
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  landMesh.SetColor(WIREFRAME_LAND_COLOR, WIREFRAME_LAND_COLOR, WIREFRAME_LAND_COLOR, WIREFRAME_LAND_COLOR);
  landMesh.Draw(LandWF);
  glEnd();

  // Land (all - solid)
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  landMesh.SetColor(LAND_COLOR, LAND_COLOR, LAND_COLOR, LAND_COLOR);
  landMesh.Draw(Land);
  glEnd();

  // Water
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  waterMesh.SetColor(WATER_COLOR, WATER_COLOR, WATER_COLOR, WATER_COLOR);
  waterMesh.Draw(Water);
  glEnd();
}

void CTerrain::MouseMove(float x, float y) {
  VECTOR4D worldRayOrigin, worldRayDir;
  VECTOR4D modelRayOrigin, modelRayDir;
  MATRIX4D InvW, Land;
  multimap<float, CMesh::INTERSECTIONINFO> faces;

  this->x = x;
  this->y = y;

  Land = *S * *P * *V * TL;

  BuildRayFromPerspective(Land, x, y, worldRayOrigin, worldRayDir);
  Inverse(Identity(), InvW);
  modelRayOrigin = InvW * worldRayOrigin;
  modelRayDir = InvW * worldRayDir;
  faceSelected = landMesh.RayCast(modelRayOrigin, modelRayDir, faces);

  if (faceSelected) {
    selectedFace = faces.begin()->second.Face;
    //selectedFaceLocation = Land * landMesh.m_Vertices[landMesh.m_Indices[selectedFace]].Position;
    //selectedFaceLocation = landMesh.m_Vertices[landMesh.m_Indices[selectedFace]].Position;
    selectedFaceLocation = faces.begin()->second.LocalPosition;
  }
}

void CTerrain::Press() {
  if (faceSelected) {
    int center = selectedFace;
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
  if (faceSelected) {
    int center = selectedFace;
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

BOOL CTerrain::GetSelectedFaceLocation(VECTOR4D& location) {
  if (faceSelected)
    location = selectedFaceLocation;
  return faceSelected;
}

CTerrain::~CTerrain() {
}

// EOF
