//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 07 - Vertex selection (sphere magnetic field)
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include <vector>
#include <map>
#include "Matrix4D.h"

using namespace std;


class CMesh {
public:
  struct VERTEX {
    VECTOR4D Position;
    VECTOR4D Normal;
    VECTOR4D Color;
  };
  struct INTERSECTIONINFO {
    int Face;
    VECTOR4D LocalPosition;
  };
  vector<VERTEX> m_Vertices;
  vector<unsigned long> m_Indices;

  CMesh();
  void Draw(MATRIX4D &M);
  void Draw(MATRIX4D &M, int startFace, int faces);
  void Press(int startFace, int faces);
  void Raise(int startFace, int faces);
  bool RayCast(VECTOR4D &RayOrigin, VECTOR4D &RayDir, multimap<float, INTERSECTIONINFO> &Faces);
  bool RayCast(VECTOR4D &RayOrigin, VECTOR4D &RayDir, multimap<float, unsigned long> &Vertices, float radius);
  void VertexShade(VERTEX(*pVS)(VERTEX V));
  ~CMesh();
};

// EOF
