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

class CFlower {
private:
  CMeshMathSurface mesh;
  MATRIX4D* V;
  MATRIX4D* P;
  MATRIX4D* S;
  vector<MATRIX4D> Ts;
public:
  CFlower(unsigned long nVx, unsigned long nVy, float x0, float y0, float du, float dv, MATRIX4D* Vi, MATRIX4D* Pe, MATRIX4D* SA);
  void Draw();
  void Add(VECTOR4D V);
  void Delete();
  ~CFlower();
};

// EOF
