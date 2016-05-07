#pragma once

#include "Matrix4D.h"
#include "Terrain.h"

class CWorld {
private:
  VECTOR4D Target = { 0.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D Eye = { 3.0f, 3.0f, 3.0f, 1.0f };
  VECTOR4D Up = { 0.0f, 0.0f, 1.0f, 0.0f };
  MATRIX4D SAspect;
  MATRIX4D P;
  MATRIX4D V;
  void DrawAxes(float length);
  BOOL SetupPixelFormat();
  CTerrain* terrain;
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
  void SetAreaSize(AreaSize size);
  ~CWorld();
};

// EOF
