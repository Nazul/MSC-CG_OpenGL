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
      VECTOR4D &Tex = m_Vertices[m_Indices[i + j]].TexCoord;
      VECTOR4D V = M * m_Vertices[m_Indices[i + j]].Position;
      glColor4f(C.r, C.g, C.b, C.a);
      glTexCoord2f(Tex.x, Tex.y);
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
      VECTOR4D &Tex = m_Vertices[m_Indices[i + j]].TexCoord;
      VECTOR4D V = M * m_Vertices[m_Indices[i + j]].Position;
      glColor4f(C.r, C.g, C.b, C.a);
      glTexCoord2f(Tex.x, Tex.y);
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

void CMesh::BuildCube() {
  VECTOR4D w = { 1, 1, 1, 1 };

  m_Vertices.clear();
  m_Vertices.resize(24);
  for (size_t i = 0; i < 24; i++) {
    m_Vertices[i].Color = w;
  }
  // 1
  m_Vertices[0].Position = { 1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[1].Position = { 1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[2].Position = { 1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[3].Position = { 1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[0].TexCoord = { 0.6640625f, 0.25f, 0, 0 };
  m_Vertices[1].TexCoord = { 0.33203125f, 0.25f, 0, 0 };
  m_Vertices[2].TexCoord = { 0.33203125f, 0.5f, 0, 0 };
  m_Vertices[3].TexCoord = { 0.6640625f, 0.5f, 0, 0 };
  // 2
  m_Vertices[4].Position = { 1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[5].Position = { 1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[6].Position = { -1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[7].Position = { -1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[4].TexCoord = { 0.6640625f, 0.5f, 0, 0 };
  m_Vertices[5].TexCoord = { 0.33203125f, 0.5f, 0, 0 };
  m_Vertices[6].TexCoord = { 0.33203125f, 0.75f, 0, 0 };
  m_Vertices[7].TexCoord = { 0.6640625f, 0.75f, 0, 0 };
  // 3
  m_Vertices[ 8].Position = { -1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[ 9].Position = { 1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[10].Position = { 1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[11].Position = { -1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[ 8].TexCoord = { 0.998046875f, 0.25f, 0, 0 };
  m_Vertices[ 9].TexCoord = { 0.6640625f, 0.25f, 0, 0 };
  m_Vertices[10].TexCoord = { 0.6640625f, 0.5f, 0, 0 };
  m_Vertices[11].TexCoord = { 0.998046875f, 0.5f, 0, 0 };
  // 4
  m_Vertices[12].Position = { 1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[13].Position = { -1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[14].Position = { -1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[15].Position = { 1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[12].TexCoord = { 0.33203125f, 0.25f, 0, 0 };
  m_Vertices[13].TexCoord = { 0, 0.25f, 0, 0 };
  m_Vertices[14].TexCoord = { 0, 0.5f, 0, 0 };
  m_Vertices[15].TexCoord = { 0.33203125f, 0.5f, 0, 0 };
  // 5
  m_Vertices[16].Position = { -1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[17].Position = { -1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[18].Position = { 1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[19].Position = { 1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[16].TexCoord = { 0.6640625f, 0, 0, 0 };
  m_Vertices[17].TexCoord = { 0.33203125f, 0, 0, 0 };
  m_Vertices[18].TexCoord = { 0.33203125f, 0.25f, 0, 0 };
  m_Vertices[19].TexCoord = { 0.6640625f, 0.25f, 0, 0 };
  // 6
  m_Vertices[20].Position = { -1.0f, 1.0f, 1.0f, 1.0f };
  m_Vertices[21].Position = { -1.0f, 1.0f, -1.0f, 1.0f };
  m_Vertices[22].Position = { -1.0f, -1.0f, -1.0f, 1.0f };
  m_Vertices[23].Position = { -1.0f, -1.0f, 1.0f, 1.0f };
  m_Vertices[20].TexCoord = { 0.6640625f, 0.75f, 0, 0 };
  m_Vertices[21].TexCoord = { 0.33203125f, 0.75f, 0, 0 };
  m_Vertices[22].TexCoord = { 0.33203125f, 0.998046875f, 0, 0 };
  m_Vertices[23].TexCoord = { 0.6640625f, 0.998046875f, 0, 0 };

  m_Indices.clear();
  //1
  m_Indices.push_back(0); m_Indices.push_back(1); m_Indices.push_back(2); m_Indices.push_back(3);
  //2
  m_Indices.push_back(4); m_Indices.push_back(5); m_Indices.push_back(6); m_Indices.push_back(7);
  //3
  m_Indices.push_back(8); m_Indices.push_back(9); m_Indices.push_back(10); m_Indices.push_back(11);
  //4
  m_Indices.push_back(12); m_Indices.push_back(13); m_Indices.push_back(14); m_Indices.push_back(15);
  //5
  m_Indices.push_back(16); m_Indices.push_back(17); m_Indices.push_back(18); m_Indices.push_back(19);
  //6
  m_Indices.push_back(20); m_Indices.push_back(21); m_Indices.push_back(22); m_Indices.push_back(23);
}

void CMesh::DrawCube(MATRIX4D &M) {
  for (unsigned long i = 0; i < m_Indices.size(); i += 4) {
    glBegin(GL_QUADS);
    for (unsigned long j = 0; j < 4; j++) {
      VECTOR4D &C = m_Vertices[m_Indices[i + j]].Color;
      VECTOR4D &Tex = m_Vertices[m_Indices[i + j]].TexCoord;
      VECTOR4D V = M * m_Vertices[m_Indices[i + j]].Position;
      glColor4f(C.r, C.g, C.b, C.a);
      glTexCoord2f(Tex.x, Tex.y);
      glVertex4f(V.x, V.y, V.z, V.w);
    }
    glEnd();
  }
}

void CMesh::VertexShade(VERTEX(*pVS)(VERTEX V)) {
  for (size_t i = 0; i < m_Vertices.size(); i++)
    m_Vertices[i] = pVS(m_Vertices[i]);
}

CMesh::~CMesh() {
}

// EOF
