//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "ImportedObject.h"
#include "objloader.h"
#include <GL\GL.h>
#include <GL\GLU.h>


CMesh::VERTEX MyFirstVertexShader(CMesh::VERTEX V) {
  CMesh::VERTEX Out = V;
  //VECTOR4D LightColor = { 1, 1, 1, 1 };
  VECTOR4D LightColor = V.Color;
  VECTOR4D LightDirection = { 0, 0, -1, 0 };
  float ILambert = max(0.0f, -Dot(V.Normal, LightDirection));

  Out.Color.r = ILambert * LightColor.r;
  Out.Color.g = ILambert * LightColor.g;
  Out.Color.b = ILambert * LightColor.b;
  Out.Color.a = ILambert * LightColor.a;
  return Out;
}


CImportedObject::CImportedObject(ObjectType type, MATRIX4D* Vi, MATRIX4D* Pe, MATRIX4D* SA) {
  VECTOR4D ct = { 64.0f / 256.0f, 79 / 256.0f, 36 / 256.0f, 1 };
  VECTOR4D cs = { 40.0f / 256.0f, 20 / 256.0f, 15 / 256.0f, 1 };
  VECTOR4D cb = { 73.0f / 256.0f, 56 / 256.0f, 41 / 256.0f, 1 };
  VECTOR4D cw = { 133.0f / 256.0f, 87 / 256.0f, 35 / 256.0f, 1 };

  switch (type) {
  case ObjectType::Tree:
    loadOBJ("C:\\Users\\Mario_Contreras\\Dropbox\\MSC\\21. GC\\Code\\MSC-CG_OpenGL\\Models-NoNormals\\Tree.obj", this->m_Vertices, this->m_Indices, ct);
    break;
  case ObjectType::Suzanne:
    loadOBJ("C:\\Users\\Mario_Contreras\\Dropbox\\MSC\\21. GC\\Code\\MSC-CG_OpenGL\\Models-NoNormals\\Suzanne.obj", this->m_Vertices, this->m_Indices, cs);
    break;
  case ObjectType::Bear:
    loadOBJ("C:\\Users\\Mario_Contreras\\Dropbox\\MSC\\21. GC\\Code\\MSC-CG_OpenGL\\Models-NoNormals\\Bear.obj", this->m_Vertices, this->m_Indices, cb);
    break;
  case ObjectType::Wood:
    loadOBJ("C:\\Users\\Mario_Contreras\\Dropbox\\MSC\\21. GC\\Code\\MSC-CG_OpenGL\\Models-NoNormals\\Wood.obj", this->m_Vertices, this->m_Indices, cw);
    break;
  }
  V = Vi;
  P = Pe;
  S = SA;
  this->type = type;

  //VertexShade(MyFirstVertexShader);
}

void CImportedObject::Draw() {
  if (Ts.size() > 0) {
    for each (MATRIX4D T in Ts) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBegin(GL_TRIANGLES);
      //glBegin(GL_POLYGON);
      CMesh::Draw(*S * *P * *V * T);
      glEnd();
    }
  }
}

void CImportedObject::Add(VECTOR4D V) {
  MATRIX4D T;

  switch (type) {
  case ObjectType::Tree:
    T = Identity() * Scaling(0.05f, 0.05f, 0.05f) * Translation(V.x * 20, V.y * 20, V.z + 3) * RotationX(-1.5708f);
    break;
  case ObjectType::Suzanne:
    T = Identity() * Scaling(0.05f, 0.05f, 0.05f) * Translation(V.x * 20, V.y * 20, V.z + 2) * RotationZ(3.14159f) * RotationX(-1.5708f);
    break;
  case ObjectType::Bear:
    T = Identity() * Scaling(0.05f, 0.05f, 0.05f) * Translation(V.x * 20 + 1, V.y * 20 + 1, V.z + 2) * RotationX(-1.5708f);
    break;
  case ObjectType::Wood:
    T = Identity() * Scaling(0.05f, 0.05f, 0.05f) * Translation(V.x * 20, V.y * 20, V.z + 1) * RotationX(-1.5708f);
    break;
  }
  Ts.push_back(T);
}

void CImportedObject::Delete() {
}

CImportedObject::~CImportedObject() {
}

// EOF
