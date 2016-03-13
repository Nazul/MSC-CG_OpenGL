//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 04 - Analytic and Parametric Surfaces
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "Matrix4D.h"
#include <vector>

using namespace std;


class CMesh {
public:
  struct VERTEX {
    VECTOR4D Position;
    VECTOR4D Color;
  };
  vector<VERTEX> m_Vertices;
  vector<unsigned long> m_Indices;

  CMesh();
  void Draw(MATRIX4D &M);
  ~CMesh();
};

// EOF
