//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "World.h"
#include <GL\GL.h>
#include <GL\GLU.h>
#include "Mesh.h"
#include "ImportedObject.h"


BOOL CWorld::SetupPixelFormat() {
  static PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
    PFD_SUPPORT_OPENGL |            // support OpenGL
    PFD_DOUBLEBUFFER,               // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    24,                             // 24-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    32,                             // 32-bit z-buffer
    0,                              // no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
  };
  int pixelformat;

  if ((pixelformat = ChoosePixelFormat(m_pDC, &pfd)) == 0) {
    return FALSE;
  }

  if (SetPixelFormat(m_pDC, pixelformat, &pfd) == FALSE) {
    return FALSE;
  }

  return TRUE;
}

CWorld::CWorld() {
}

void CWorld::Init() {
  PIXELFORMATDESCRIPTOR pfd;
  int         n;
  HGLRC       hrc;
  GLfloat     fMaxObjSize, fAspect;
  GLfloat     fNearPlane, fFarPlane;
  RECT rect;
  float radius;

  m_pDC = GetDC(hWnd);

  if (!SetupPixelFormat())
    return;

  n = ::GetPixelFormat(m_pDC);
  ::DescribePixelFormat(m_pDC, n, sizeof(pfd), &pfd);

  hrc = wglCreateContext(m_pDC);
  wglMakeCurrent(m_pDC, hrc);

  GetClientRect(hWnd, &rect);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (rect.bottom)
    fAspect = (GLfloat)rect.right / rect.bottom;
  else    // don't divide by zero, not that we should ever run into that...
    fAspect = 1.0f;
  fNearPlane = 3.0f;
  fFarPlane = 7.0f;
  fMaxObjSize = 3.0f;
  radius = fNearPlane + fMaxObjSize / 2.0f;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
  glMatrixMode(GL_MODELVIEW);

  // Prepare view
  V = View(Eye, Target, Up);
  // Prepare perspective
  P = PerspectiveWidthHeightRH(0.5f, 0.5f, 2.5f, 10.0f);

  // Objects
  terrain = new CTerrain(64, 64, -1, -1, 2.0f / (64 - 1), 2.0f / (64 - 1), &V, &P, &SAspect);
  terrain->SetArea(AreaSize::Small);
  flowers = new CFlower(100, 30, -1, -1, 1.0f / (10 - 1), 1.0f / (10 - 1), &V, &P, &SAspect);
  // Imported Objects
  trees = new CImportedObject(ObjectType::Tree, &V, &P, &SAspect);
  bears = new CImportedObject(ObjectType::Bear, &V, &P, &SAspect);
  woods = new CImportedObject(ObjectType::Wood, &V, &P, &SAspect);
  suzannes = new CImportedObject(ObjectType::Suzanne, &V, &P, &SAspect);
}

CWorld* CWorld::CreateWorld(HWND hWnd) {
  CWorld* newWorld = new CWorld();

  newWorld->hWnd = hWnd;
  newWorld->Init();

  return newWorld;
}

void CWorld::Display() {
  static BOOL bBusy = FALSE;

  if (bBusy)
    return;
  bBusy = TRUE;

  //// clear the window
  //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearColor(0.529411765f, 0.807843137f, 0.980392157f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Load identity
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  terrain->Draw();
  flowers->Draw();
  trees->Draw();
  bears->Draw();
  woods->Draw();
  suzannes->Draw();

  // show the rendering on the screen
  SwapBuffers(wglGetCurrentDC());

  bBusy = FALSE;
}

void CWorld::Resize(int w, int h) {
  this->w = w;
  this->h = h;
  glViewport(0, 0, w, h);
  SAspect = Scaling((float)h / w, 1, 1);
  //SAspect = Scaling(1, (float)w / h, 1);
}

void CWorld::MouseMove(int x, int y) {
  terrain->MouseMove(-1 + 2 * (float)x / w, 1 - 2 * (float)y / h);
}

void CWorld::Press() {
  terrain->Press();
}

void CWorld::Raise() {
  terrain->Raise();
}

void CWorld::AddFlower() {
  VECTOR4D V;

  if (terrain->GetSelectedFaceLocation(V))
    flowers->Add(V);
}

void CWorld::DeleteFlower() {
}

void CWorld::AddImportedObject(ObjectType type) {
  VECTOR4D V;

  if (terrain->GetSelectedFaceLocation(V)) {
    switch (type) {
    case ObjectType::Tree:
      trees->Add(V);
      break;
    case ObjectType::Suzanne:
      suzannes->Add(V);
      break;
    case ObjectType::Bear:
      bears->Add(V);
      break;
    case ObjectType::Wood:
      woods->Add(V);
      break;
    }
  }
}

void CWorld::DeleteImportedObject() {
}

void CWorld::SetAreaSize(AreaSize size) {
  terrain->SetArea(size);
}

CWorld::~CWorld() {
}

// EOF
