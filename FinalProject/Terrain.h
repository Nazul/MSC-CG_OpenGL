//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "MeshMathSurface.h"


enum class AreaSize { Single, Small, Medium, Large };

class CTerrain {
private:
  CMeshMathSurface landMesh;
  CMeshMathSurface waterMesh;
  MATRIX4D TL;
  MATRIX4D TW;
  MATRIX4D* V;
  MATRIX4D* P;
  MATRIX4D* S;
  float x, y;
  unsigned long h, w;
  AreaSize size;
  BOOL faceSelected = FALSE;
  VECTOR4D selectedFaceLocation;
  int selectedFace;
public:
  CTerrain(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, MATRIX4D* Vi, MATRIX4D* Pe, MATRIX4D* SA);
  void Draw();
  void MouseMove(float x, float y);
  void Press();
  void Raise();
  void SetArea(AreaSize size);
  BOOL GetSelectedFaceLocation(VECTOR4D& location);
  ~CTerrain();
};

// EOF
