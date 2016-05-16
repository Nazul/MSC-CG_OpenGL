//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "Mesh.h"

enum class ObjectType { Tree, Suzanne, Bear, Wood };


class CImportedObject : public CMesh {
private:
  MATRIX4D* V;
  MATRIX4D* P;
  MATRIX4D* S;
  vector<MATRIX4D> Ts;
  ObjectType type;
public:
  CImportedObject(ObjectType type, MATRIX4D* Vi, MATRIX4D* Pe, MATRIX4D* SA);
  void Draw();
  void Add(VECTOR4D V);
  void Delete();
  ~CImportedObject();
};

// EOF
