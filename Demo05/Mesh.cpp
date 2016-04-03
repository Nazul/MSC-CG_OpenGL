//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 05 - Mouse & Raycast
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "Mesh.h"
#include "GL\glew.h"


CMesh::CMesh() {
}

void CMesh::Draw(MATRIX4D &M) {
  for (unsigned long i = 0; i < m_Indices.size(); i += 3) {
    for (unsigned long j = 0; j < 3; j++) {
      VECTOR4D &C = m_Vertices[m_Indices[i + j]].Color;
      VECTOR4D V = M * m_Vertices[m_Indices[i + j]].Position;
      glColor4f(C.r, C.g, C.b, C.a);
      glVertex4f(V.x, V.y, V.z, V.w);
    }
  }
}

bool CMesh::RayCast(VECTOR4D &RayOrigin, VECTOR4D &RayDir, multimap<float, unsigned long>& Faces) {
  unsigned long nFaces = (unsigned long)m_Indices.size() / 3;
  unsigned long nBaseIndex = 0;
  unsigned long nIntersectedFaces = 0;

  for (unsigned long iFace = 0; iFace < nFaces; iFace++) {
    VECTOR4D &V0 = m_Vertices[m_Indices[nBaseIndex + 0]].Position;
    VECTOR4D &V1 = m_Vertices[m_Indices[nBaseIndex + 1]].Position;
    VECTOR4D &V2 = m_Vertices[m_Indices[nBaseIndex + 2]].Position;
    VECTOR4D Intersection;

    if (RayCastOnTriangle(V0, V1, V2, RayOrigin, RayDir, Intersection)) {
      float dist = Magnity(Intersection - RayOrigin);
      Faces.insert(make_pair(dist, iFace));
      nIntersectedFaces++;
    }
    nBaseIndex += 3;
  }
  return nIntersectedFaces > 0;
}

CMesh::~CMesh() {
}

// EOF
