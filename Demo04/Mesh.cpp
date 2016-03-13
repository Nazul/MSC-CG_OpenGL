//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 04 - Analytic and Parametric Surfaces
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

CMesh::~CMesh() {
}

// EOF
