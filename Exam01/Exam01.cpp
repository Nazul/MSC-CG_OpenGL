//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Exam 01
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include <math.h>
#include <string>
#include "GL\glew.h"
#include "GL\glut.h"
#include "GL\freeglut.h"
#include "Matrix4D.h"
#include "MeshMathSurface.h"
#include <iostream>

#define REFRESH_MILISECS 100
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define INCREMENT 0.01f
#define PI 3.141592f

using namespace std;

bool bRotateX, bRotateY, bRotateZ, bScaleUp, bScaleDown, bForward, bBackward, bLeft, bRight, bUp, bDown, bWireframe = true;
bool bDunes, bSquares, bWater, bCone, bToroid, bCylinder, bSphere;

float mx, my; // Normalized mouse coords

MATRIX4D T;
MATRIX4D R;
MATRIX4D Vi;
CMeshMathSurface g_Dunes;
CMeshMathSurface g_Squares;
CMeshMathSurface g_Water;
CMeshMathSurface g_Cone;
CMeshMathSurface g_Toroid;
CMeshMathSurface g_Cylinder;
CMeshMathSurface g_Sphere;


float Dunes(float x, float y) {
  return 0.2f * sinf(1.5f * y * PI) * cosf(1.5f * x * PI);
}

float Squares(float x, float y) {
  if ((x < 0.0f && y < 0.0f) || (x > 0.0f && y > 0.0f))
    return 0.0f;
  else
    return 0.5f;
}

float Cone(float x, float y) {
  float a, b, c;

  a = b = c = 0.8f;

  return -sqrtf((((x * x) / (a * a)) + ((y * y) / (b * b))) / (c * c));
}

float Water(float x, float y) {
  //return 0.8f * sinf(sqrtf(60.0f * ((x * x) + (y * y)))) / sqrtf(90.0f * ((x * x) + (y * y)));
  float r = sqrtf(x * x + y * y);

  return 0.2f * sinf(r * 2.0f * PI * 1.8f);
}

VECTOR4D Toroid(float u, float v) {
  VECTOR4D V;
  float a = 0.1f;
  float c = 0.4f;

  V.x = (c + a * cosf(v)) * cosf(u);
  V.y = (c + a * cosf(v)) * sinf(u);
  V.z = a * sinf(v);
  V.w = 1.0f;

  return V;
}

VECTOR4D Cylinder(float u, float v) {
  VECTOR4D V;
  float h = 0.4f;
  float r = 0.2f;

  V.x = h * cosf(v);
  V.y = h * sin(v);
  V.z = r * u;
  V.w = 1.0f;

  return V;
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

void renderScene(void) {
  int sx = glutGet(GLUT_WINDOW_WIDTH);
  int sy = glutGet(GLUT_WINDOW_HEIGHT);
  MATRIX4D SAspect = Scaling((float)sy / sx, 1, 1);
  //MATRIX4D SAspect = Scaling(1, (float)sx / sy, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (bWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBegin(GL_TRIANGLES);
  if (bDunes)
    g_Dunes.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  if (bSquares)
    g_Squares.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  if (bWater)
    g_Water.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  if (bCone)
    g_Cone.Draw(SAspect * T * Translation(0.0f, 0.0f, 1.0f));
  if (bToroid)
    g_Toroid.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  if (bCylinder)
    g_Cylinder.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  if (bSphere)
    g_Sphere.Draw(SAspect * T * Translation(0.0f, 0.0f, 0.0f));
  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  static float scale = 2.0f;
  VECTOR4D Speed = { 0.02f, 0.02f, 0.02f, 0.0f };
  MATRIX4D P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);
  MATRIX4D InvV = FastInverse(Vi);
  VECTOR4D XDir = { InvV.m00, InvV.m10, InvV.m20, 0.0f };
  VECTOR4D YDir = { InvV.m01, InvV.m11, InvV.m21, 0.0f };
  VECTOR4D ZDir = { InvV.m02, InvV.m12, InvV.m22, 0.0f };
  VECTOR4D EyePos = { InvV.m03, InvV.m13, InvV.m23, 1.0f };

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
  case 'a':
  case 'A':
    bDunes = !bDunes;
    break;
  case 'b':
  case 'B':
    bSquares = !bSquares;
    break;
  case 'c':
  case 'C':
    bWater = !bWater;
    break;
  case 'd':
  case 'D':
    bCone = !bCone;
    break;
  case 'e':
  case 'E':
    bToroid = !bToroid;
    break;
  case 'f':
  case 'F':
    bCylinder = !bCylinder;
    break;
  case 'g':
  case 'G':
    bSphere = !bSphere;
    break;
  }
  glutPostRedisplay();
}

void keyUpHandler(unsigned char key, int x, int y) {
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

void DisplayHelp() {
  cout << "Mario Contreras (705080)" << endl;
  cout << "Computer Graphics - Exam 01" << endl << endl << endl;
  cout << "Keyboard Options" << endl;
  cout << "----------------" << endl << endl;
  cout << "Esc - Exit" << endl;
  cout << "  + - Scale up (Scale Matrix)" << endl;
  cout << "  - - Scale down (Scale Matrix)" << endl;
  cout << "  x - Rotate x axis" << endl;
  cout << "  y - Rotate y axis" << endl;
  cout << "  z - Rotate z axis" << endl;
  cout << "  w - Wireframe mode on/off" << endl;
  cout << "  2 - Camera down" << endl;
  cout << "  4 - Camera left" << endl;
  cout << "  6 - Camera right" << endl;
  cout << "  8 - Camera up" << endl;
  cout << "  7 - Camera forward" << endl;
  cout << "  9 - Camera backward" << endl;
  cout << "" << endl;
  cout << "  a - Dune on/off" << endl;
  cout << "  b - Square on/off" << endl;
  cout << "  c - Water on/off" << endl;
  cout << "  d - Cone on/off" << endl;
  cout << "  e - Toroid on/off" << endl;
  cout << "  f - Cylinder on/off" << endl;
  cout << "  g - Sphere on/off" << endl;
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

  DisplayHelp();
  // Not recommended, but useful
  system("pause");

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
    glutCreateWindow("MSC GC 705080 - Exam 01");
  }

  // Initialize viewport and rotation matrix
  Vi = View(Eye, Target, Up);
  R = Identity();

  // Create surfaces
  g_Dunes.BuildAnalyticSurface(32, 32, -1.0f, -1.0f, 2.0f / (32.0f - 1.0f), 2.0f / (32.0f - 1.0f), Dunes);
  g_Dunes.SetColor(r, g, b, w);
  g_Squares.BuildAnalyticSurface(32, 32, -1.0f, -1.0f, 2.0f / (32.0f - 1.0f), 2.0f / (32.0f - 1.0f), Squares);
  g_Squares.SetColor(r, g, b, w);
  g_Water.BuildAnalyticSurface(32, 32, -1, -1, 2.0f / (32.0f - 1), 2.0f / (32.0f - 1), Water);
  g_Water.SetColor(r, g, b, w);
  g_Cone.BuildAnalyticSurface(40, 40, -1, -1, 2.0f / (40 - 1), 2.0f / (40 - 1), Cone);
  g_Cone.SetColor(r, g, b, w);
  g_Toroid.BuildParametricSurface(30, 30, -1, -1, (2.0f * PI) / (30 - 1), (2.0f * PI) / (30 - 1), Toroid);
  g_Toroid.SetColor(b, w, w, b);
  g_Cylinder.BuildParametricSurface(30, 30, -1, -1, (2.0f * PI) / (30 - 1), (2.0f * PI) / (30 - 1), Cylinder);
  g_Cylinder.SetColor(y, w, w, y);
  g_Sphere.BuildParametricSurface(30, 30, -1, -1, 1.0f / (30 - 1), 1.0f / (30 - 1), Sphere);
  g_Sphere.SetColor(r, b, b, r);

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutKeyboardUpFunc(keyUpHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  // Enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutMainLoop();

  return 0;
}

//EOF
