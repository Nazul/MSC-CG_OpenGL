//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 07 - Vertex selection (sphere magnetic field)
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "MeshMathSurface.h"


void CMeshMathSurface::Tesselate() {
  unsigned long k = 0;

  m_Indices.resize((m_nVx - 1) * (m_nVy - 1) * 3 * 2);

  for (unsigned long j = 0; j < m_nVy - 1; j++)
    for (unsigned long i = 0; i < m_nVx - 1; i++) {
      m_Indices[k + 0] = j * m_nVx + i;
      m_Indices[k + 1] = (j + 1) * m_nVx + i;
      m_Indices[k + 2] = j * m_nVx + i + 1;
      m_Indices[k + 3] = m_Indices[2 + k];
      m_Indices[k + 4] = m_Indices[1 + k];
      m_Indices[k + 5] = (j + 1) * m_nVx + i + 1;
      k += 6;
    }
}

void CMeshMathSurface::SetColor(VECTOR4D & A, VECTOR4D & B, VECTOR4D & C, VECTOR4D & D) {
  float dcx = 1.0f / m_nVx;
  float dcy = 1.0f / m_nVy;
  float cx, cy = 0.0f;

  for (unsigned long j = 0; j < m_nVy; j++) {
    cx = 0.0f;
    for (unsigned long i = 0; i < m_nVx; i++) {
      m_Vertices[j * m_nVx + i].Color = Lerp(Lerp(A, B, cx), Lerp(C, D, cx), cy);
      cx += dcx;
    }
    cy += dcy;
  }
}

CMeshMathSurface::CMeshMathSurface() {
}

void CMeshMathSurface::BuildAnalyticSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, float(*pFn)(float x, float y), VECTOR4D(*pFnDivergent)(float x, float y, float z)) {
  float x, y = y0;

  m_Indices.clear();
  m_Vertices.clear();
  m_nVx = nVx;
  m_nVy = nVy;
  m_Vertices.resize(m_nVx * m_nVy);
  for (unsigned long j = 0; j < m_nVy; j++) {
    x = x0;
    for (unsigned long i = 0; i < m_nVx; i++) {
      VECTOR4D P = { x, y, pFn(x, y), 1 };
      m_Vertices[j * m_nVx + i].Position = P;
      m_Vertices[j*m_nVx + i].Normal = pFnDivergent(x, y, P.z);
      x += dx;
    }
    y += dy;
  }
  Tesselate();
}

void CMeshMathSurface::BuildAnalyticSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float dx, float dy, float(*pFn)(float x, float y)) {
  float x, y = y0;

  m_Indices.clear();
  m_Vertices.clear();
  m_nVx = nVx;
  m_nVy = nVy;
  m_Vertices.resize(m_nVx * m_nVy);
  for (unsigned long j = 0; j < m_nVy; j++) {
    x = x0;
    for (unsigned long i = 0; i < m_nVx; i++) {
      VECTOR4D P = { x, y, pFn(x, y), 1 };
      m_Vertices[j * m_nVx + i].Position = P;
      x += dx;
    }
    y += dy;
  }
  Tesselate();
}

void CMeshMathSurface::BuildParametricSurface(unsigned long nVx, unsigned long nVy, float x0, float y0, float du, float dv, VECTOR4D(*pFn)(float u, float v)) {
  float x, y = y0;

  m_Indices.clear();
  m_Vertices.clear();
  m_nVx = nVx;
  m_nVy = nVy;
  m_Vertices.resize(m_nVx * m_nVy);
  for (unsigned long j = 0; j < m_nVy; j++) {
    x = x0;
    for (unsigned long i = 0; i < m_nVx; i++) {
      VECTOR4D P = pFn(x, y);
      m_Vertices[j * m_nVx + i].Position = P;
      x += du;
    }
    y += dv;
  }
  Tesselate();
}

CMeshMathSurface::~CMeshMathSurface() {
}

// EOF
