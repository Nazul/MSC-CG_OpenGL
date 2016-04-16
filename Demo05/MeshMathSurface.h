//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 05 - Mouse & Raycast
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "Mesh.h"


class CMeshMathSurface : public CMesh {
protected:
  unsigned long m_nVx;
  unsigned long m_nVy;

  void Tesselate();

public:
  CMeshMathSurface();
  void BuildAnalyticSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, float(*pFn)(float x, float y), VECTOR4D(*pFnDivergent)(float x, float y, float z));
  void BuildAnalyticSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, float(*pFn)(float x, float y));
  void BuildParametricSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float du, float dv, VECTOR4D(*pFn)(float u, float v));
  void SetColor(VECTOR4D &A, VECTOR4D &B, VECTOR4D &C, VECTOR4D &D);
  ~CMeshMathSurface();
};

// EOF
