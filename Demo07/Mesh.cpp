//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 07 - Vertex selection (sphere magnetic field)
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

void CMesh::Draw(MATRIX4D &M, int startFace, int faces) {
  int totalFaces = (int)m_Indices.size() / 3;

  if (faces == -1)
    faces = totalFaces;
  if (startFace + faces > totalFaces)
    faces = totalFaces - startFace;
  if (faces < 0)
    return;

  for (unsigned long i = startFace * 3; faces--; i += 3) {
    for (unsigned long j = 0; j < 3; j++) {
      VECTOR4D &C = m_Vertices[m_Indices[i + j]].Color;
      VECTOR4D V = M * m_Vertices[m_Indices[i + j]].Position;
      glColor4f(C.r, C.g, C.b, C.a);
      glVertex4f(V.x, V.y, V.z, V.w);
    }
  }
}

bool CMesh::RayCast(VECTOR4D &RayOrigin, VECTOR4D &RayDir, multimap<float, INTERSECTIONINFO>& Faces) {
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
      INTERSECTIONINFO II;
      II.Face = iFace;
      II.LocalPosition = Intersection;
      Faces.insert(make_pair(dist, II));
      nIntersectedFaces++;
    }
    nBaseIndex += 3;
  }
  return nIntersectedFaces > 0;
}

bool RaySphereIntersect(VECTOR4D &RayOrigin, VECTOR4D &RayDirection, VECTOR4D &SphereCenter, float r) {
  VECTOR4D RO = RayOrigin - SphereCenter;
  float a = Dot(RayDirection, RayDirection);
  float b = 2 * Dot(RayDirection, RO);
  float c = Dot(RO, RO) - r * r;
  float disc = b * b - 4 * a * c;

  if (disc < 0)
    return false;
  return true;
}

bool CMesh::RayCast(VECTOR4D & RayOrigin, VECTOR4D & RayDir, multimap<float, unsigned long>& Vertices, float radius) {
  for (size_t i = 0; i < m_Vertices.size(); i++) {
    if (RaySphereIntersect(RayOrigin, RayDir, m_Vertices[i].Position, radius)) {
      float dist = Magnity(m_Vertices[i].Position - RayOrigin);
      Vertices.insert(make_pair(dist, (unsigned long)i));
    }
  }
  return Vertices.size() > 0;
}

void CMesh::VertexShade(VERTEX(*pVS)(VERTEX V)) {
  for (size_t i = 0; i < m_Vertices.size(); i++)
    m_Vertices[i] = pVS(m_Vertices[i]);
}

CMesh::~CMesh() {
}

// EOF
