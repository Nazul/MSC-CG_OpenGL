//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 05 - Mouse & Raycast
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include <math.h>
#include <string>
#include <algorithm>
#include "GL\glew.h"
#include "GL\glut.h"
#include "GL\freeglut.h"
#include "Matrix4D.h"
#include "MeshMathSurface.h"
#ifdef _DEBUG
#include <iostream>
#endif

#define REFRESH_MILISECS 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define INCREMENT 0.01f
#define PI 3.141592f

bool bRotateX, bRotateY, bRotateZ, bScaleUp, bScaleDown, bForward, bBackward, bLeft, bRight, bUp, bDown, bWireframe = false;

float mx, my; // Normalized mouse coords

MATRIX4D T;
MATRIX4D R;
MATRIX4D P;
MATRIX4D Vi;
CMeshMathSurface g_EggCarton;
CMeshMathSurface g_Plate;
CMeshMathSurface g_Sphere;
CMeshMathSurface g_Bananas;
CMeshMathSurface g_Flower;


float SinCos(float x, float y) {
  return 0.3f * cosf(3.0f * x * PI) * sinf(3.0f * y * PI);
}

VECTOR4D SinCosNormal(float x, float y, float z) {
  VECTOR4D N = { 0.3f * 3.0f * PI * sin(3.0f * x * PI) * sinf(3.0f * y * PI), -0.3f * 3.0f * PI * cosf(3.0f * x * PI) * cos(3.0f * y * PI), 1.0f, 0.0f };
  N = Normalize(N);
  return N;
}

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

float x2y2(float x, float y) {
  return 0.4f * x * x * y * y;
}

VECTOR4D Sphere(float u, float v) {
  VECTOR4D V;
  float r = 0.4f;

  V.x = r * (cosf(2.0f * PI * u) * sinf(PI * v));
  V.y = r * (sinf(2.0f * PI * u) * sinf(PI * v));
  V.z = r * (cosf(PI * v));
  V.w = 1.0f;

  return V;
}

VECTOR4D Bananas(float u, float v) {
  VECTOR4D V;

  V.x = 0.2f * ((2.0f + sinf(2 * PI * v) * sinf(2.0f * PI * u)) * sinf(3.0f * PI * v));
  V.y = 0.2f * (sinf(2.0f * PI * v) * cosf(2.0f * PI * u) + 4.0f * v - 2.0f);
  V.z = 0.2f * ((2.0f + sinf(2.0f * PI * v) * sinf(2.0f * PI * u)) * cosf(3.0f * PI * v));
  V.w = 1.0f;

  return V;
}

VECTOR4D Dini(float u, float v) {
  VECTOR4D V;
  float a = 1.0f;
  float b = 0.2f;

  V.x = 0.5f * (a * cosf(u) * sinf(v));
  V.y = 0.5f * (a * sinf(u) * sinf(v));
  V.z = 0.5f * (a * (cosf(v) + logf(tanf(v * 0.5f))) + b * u);
  V.w = 1.0f;

  return V;
}

void renderScene(void) {
  int sx = glutGet(GLUT_WINDOW_WIDTH);
  int sy = glutGet(GLUT_WINDOW_HEIGHT);
  MATRIX4D SAspect = Scaling((float)sy / sx, 1, 1);
  //MATRIX4D SAspect = Scaling(1, (float)sx / sy, 1);
  VECTOR4D worldRayOrigin, worldRayDir;
  VECTOR4D modelRayOrigin, modelRayDir;
  MATRIX4D W = Identity();
  MATRIX4D InvW;
  multimap<float, CMesh::INTERSECTIONINFO> faces;
  MATRIX4D temp;
  bool fill = false;

  P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);
  //temp = SAspect * P * Vi;
  temp = SAspect * T * Translation(0.0f, 0.0f, -1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  BuildRayFromPerspective(temp, mx, my, worldRayOrigin, worldRayDir);
  Inverse(W, InvW);
  modelRayOrigin = InvW * worldRayOrigin;
  modelRayDir = InvW * worldRayDir;
  fill = g_EggCarton.RayCast(modelRayOrigin, modelRayDir, faces);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  g_EggCarton.Draw(SAspect * T * Translation(0.0f, 0.0f, -1.0f));
  //g_EggCarton.Draw(temp * W);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  if (fill)
    g_EggCarton.Draw(SAspect * T * Translation(0.0f, 0.0f, -1.0f), faces.begin()->second.Face, 1);
    //g_EggCarton.Draw(temp * W, faces.begin()->second.Face, 1);
  glEnd();

  if (bWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  g_Plate.Draw(SAspect * T * Translation(0.0f, 0.0f, -0.1f));
  g_Sphere.Draw(SAspect * T * Translation(0.4f, 0.4f, 0.32f));
  g_Bananas.Draw(SAspect * T * Translation(0.45f, 0.45f, 0.48f));
  g_Flower.Draw(SAspect * T * Translation(-0.5f, 0.5f, 0.8f));
  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  static float scale = 1.0f;
  VECTOR4D Speed = { 0.02f, 0.02f, 0.02f, 0.0f };
  //MATRIX4D P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);
  MATRIX4D InvV = FastInverse(Vi);
  VECTOR4D XDir = { InvV.m00, InvV.m10, InvV.m20, 0.0f };
  VECTOR4D YDir = { InvV.m01, InvV.m11, InvV.m21, 0.0f };
  VECTOR4D ZDir = { InvV.m02, InvV.m12, InvV.m22, 0.0f };
  VECTOR4D EyePos = { InvV.m03, InvV.m13, InvV.m23, 1.0f };

  P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);

  if (bLeft)
    EyePos = EyePos + XDir * Speed;
  if (bRight)
    EyePos = EyePos - XDir * Speed;
  if (bDown)
    EyePos = EyePos + YDir * Speed;
  if (bUp)
    EyePos = EyePos - YDir * Speed;
  if (bBackward)
    EyePos = EyePos + ZDir * Speed;
  if (bForward)
    EyePos = EyePos - ZDir * Speed;
  InvV.m03 = EyePos.x;
  InvV.m13 = EyePos.y;
  InvV.m23 = EyePos.z;
  Vi = FastInverse(InvV);

  if (bRotateX)
    R = R * RotationX(INCREMENT);
  if (bRotateY)
    R = R * RotationY(INCREMENT);
  if (bRotateZ)
    R = R * RotationZ(INCREMENT);
  if (bScaleUp)
    scale += INCREMENT;
  if (bScaleDown)
    scale -= INCREMENT;

  T = (P * Vi) * R * Scaling(scale, scale, scale);

  glutPostRedisplay();
}

void keyHandler(unsigned char key, int x, int y) {
#ifdef _DEBUG
  std::cout << key << " pressed" << std::endl;
#endif
  switch (key) {
  case 27:
    glutLeaveMainLoop();
    break;
  case '+':
    bScaleUp = true;
    break;
  case '-':
    bScaleDown = true;
    break;
  case '2':
    bDown = true;
    break;
  case '4':
    bLeft = true;
    break;
  case '6':
    bRight = true;
    break;
  case '7':
    bBackward = true;
    break;
  case '8':
    bUp = true;
    break;
  case '9':
    bForward = true;
    break;
    case 'W':
    case 'w':
      bWireframe = !bWireframe;
      break;
  case 'X':
  case 'x':
    bRotateX = !bRotateX;
    break;
  case 'Y':
  case 'y':
    bRotateY = !bRotateY;
    break;
  case 'Z':
  case 'z':
    bRotateZ = !bRotateZ;
    break;
  }
  glutPostRedisplay();
}

void keyUpHandler(unsigned char key, int x, int y) {
#ifdef _DEBUG
  std::cout << key << " released" << std::endl;
#endif
  switch (key) {
  case '+':
    bScaleUp = false;
    break;
  case '-':
    bScaleDown = false;
    break;
  case '2':
    bDown = false;
    break;
  case '4':
    bLeft = false;
    break;
  case '6':
    bRight = false;
    break;
  case '7':
    bBackward = false;
    break;
  case '8':
    bUp = false;
    break;
  case '9':
    bForward = false;
    break;
  }
  glutPostRedisplay();
}

void onPassiveMotion(int x, int y) {
#ifdef _DEBUG
  std::cout << "X: " << x << " Y: " << y << std::endl;
#endif

  mx = -1.0f + 2.0f * (float)x / glutGet(GLUT_WINDOW_WIDTH);
  my = 1.0f - 2.0f * (float)y / glutGet(GLUT_WINDOW_HEIGHT);

#ifdef _DEBUG
  std::cout << "mx: " << mx << " my: " << my << std::endl;
#endif
}


int main(int argc, char **argv) {
  bool gameMode = false;
  int startX = 100, startY = 100;
  VECTOR4D Target = { 0.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D Eye = { 3.0f, 3.0f, 3.0f, 1.0f };
  VECTOR4D Up = { 0.0f, 0.0f, 1.0f, 0.0f };
  VECTOR4D r = { 1, 0, 0, 1 };
  VECTOR4D g = { 0, 1, 0, 1 };
  VECTOR4D b = { 0, 0, 1, 1 };
  VECTOR4D w = { 1, 1, 1, 1 };
  VECTOR4D y = { 1, 1, 0, 1 };

  VECTOR4D V1 = { 0,  1, 0, 1 };
  VECTOR4D V2 = { 1, -1, 0, 1 };
  VECTOR4D V3 = { -1, -1, 0, 1 };
  VECTOR4D P1 = { 0, -1.1f, 0, 1 };

  // Init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  if (argc > 1) {
    if (std::string(argv[1]) == "-game") {
      glutGameModeString("1024x768:32");
      if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
        gameMode = true;
        glutEnterGameMode();
      }
    }
    if (std::string(argv[1]) == "-center") {
      startX = glutGet(GLUT_SCREEN_WIDTH) / 2 - WINDOW_WIDTH / 2;
      startY = glutGet(GLUT_SCREEN_HEIGHT) / 2 - WINDOW_HEIGHT / 2;
    }
  }
  if (!gameMode) {
    glutInitWindowPosition(startX, startY);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("MSC GC 705080 - Demo 5 (Mouse and raycast)");
  }

  // Initialize viewport and rotation matrix
  Vi = View(Eye, Target, Up);
  R = Identity();

  // Create surfaces
  g_EggCarton.BuildAnalyticSurface(32, 32, -1, -1, 2.0f / (32 - 1), 2.0f / (32 - 1), SinCos, SinCosNormal);
  g_EggCarton.SetColor(r, g, b, w);
  g_EggCarton.VertexShade(MyFirstVertexShader);
  g_Plate.BuildAnalyticSurface(40, 40, -1, -1, 2.0f / (40 - 1), 2.0f / (40 - 1), x2y2);
  g_Plate.SetColor(r, g, b, w);
  g_Sphere.BuildParametricSurface(30, 30, -1, -1, 1.0f / (30 - 1), 1.0f / (30 - 1), Sphere);
  g_Sphere.SetColor(r, b, b, r);
  g_Flower.BuildParametricSurface(100, 30, -1, -1, 1.0f / (10 - 1), 1.0f / (10 - 1), Dini);
  g_Flower.SetColor(b, w, w, b);
  g_Bananas.BuildParametricSurface(30, 30, -1, -1, 1.0f / (30 - 1), 1.0f / (30 - 1), Bananas);
  g_Bananas.SetColor(y, w, w, y);

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutKeyboardUpFunc(keyUpHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);
  glutPassiveMotionFunc(onPassiveMotion);

  // Enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}

//EOF
