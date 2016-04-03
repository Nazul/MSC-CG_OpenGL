//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 05 - Mouse & Raycast
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "Matrix4D.h"
#include <vector>
#include <map>

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
  bool RayCast(VECTOR4D &RayOrigin, VECTOR4D &RayDir, multimap<float, unsigned long>& Faces);
  ~CMesh();
};

// EOF
