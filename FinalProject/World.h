//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once

#include "Matrix4D.h"
#include "Terrain.h"
#include "Flower.h"
#include "ImportedObject.h"

enum class Operation { Terrain, Flower, ImportedObject };


class CWorld {
private:
  VECTOR4D Target = { 0.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D Eye = { 3.0f, 3.0f, 3.0f, 1.0f };
  VECTOR4D Up = { 0.0f, 0.0f, 1.0f, 0.0f };
  MATRIX4D SAspect;
  MATRIX4D P;
  MATRIX4D V;
  BOOL SetupPixelFormat();
  CTerrain* terrain;
  CFlower* flowers;
  CImportedObject* trees;
  CImportedObject* suzannes;
  CImportedObject* bears;
  CImportedObject* woods;
protected:
  int w, h;
  HWND hWnd;
  HDC m_pDC;
  CWorld();
  void Init();
public:
  static CWorld* CreateWorld(HWND hWnd);
  void Display();
  void Resize(int w, int h);
  void MouseMove(int x, int y);
  void Press();
  void Raise();
  void AddFlower();
  void DeleteFlower();
  void AddImportedObject(ObjectType type);
  void DeleteImportedObject();
  void SetAreaSize(AreaSize size);
  ~CWorld();
};

// EOF
