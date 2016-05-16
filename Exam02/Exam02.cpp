//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 07 - Vertex selection (sphere magnetic field)
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
#include "ImageBMP.h"
#ifdef _DEBUG
#include <iostream>
#endif

#define REFRESH_MILISECS 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define INCREMENT 0.05f
#define PI 3.141592f

bool bRotateX, bRotateY, bRotateZ, bScaleUp, bScaleDown, bForward, bBackward, bLeft, bRight, bUp, bDown, bWireframe, bVertexSelected;
unsigned long ulVertexSelectedIndex;

float mx, my; // Normalized mouse coords

MATRIX4D SAspect;
MATRIX4D P;
MATRIX4D Vi;
MATRIX4D T;
MATRIX4D R;
MATRIX4D W;
MATRIX4D EC;
CMesh g_Cube;

GLuint TexDiffuse;  //Recurso de textura difusa.


VECTOR4D Cube(float u, float v) {
  VECTOR4D V;
  float a = 1.0f;

  V.x = (a * sinf(u) * cosf(v)) / (pow(pow(sinf(u), 6) * (pow(sinf(v), 6) + pow(cosf(v), 6)) + pow(cosf(u), 6), 1.0f / 6.0f));
  V.y = (a * sinf(u) * sinf(v)) / (pow(pow(sinf(u), 6) * (pow(sinf(v), 6) + pow(cosf(v), 6)) + pow(cosf(u), 6), 1.0f / 6.0f));
  V.z = (a * cosf(u)) / (pow(pow(sinf(u), 6) * (pow(sinf(v), 6) + pow(cosf(v), 6)) + pow(cosf(u), 6), 1.0f / 6.0f));
  V.w = 1.0f;

  return V;
}

float r2() {
  return (float)rand() / (float)RAND_MAX;
}

void renderScene(void) {
  int sx = glutGet(GLUT_WINDOW_WIDTH);
  int sy = glutGet(GLUT_WINDOW_HEIGHT);

  SAspect = Scaling((float)sy / sx, 1, 1);
  W = Identity();

  P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);
  EC = SAspect * T * Translation(0.0f, 0.0f, -1.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (bWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBindTexture(GL_TEXTURE_2D, TexDiffuse);
  glEnable(GL_TEXTURE_2D);
  g_Cube.DrawCube(SAspect * T * Translation(0.0f, 0.0f, 0.0f));

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  static float scale = 1.0f;
  VECTOR4D Speed = { 0.02f, 0.02f, 0.02f, 0.0f };
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

  if (bRotateX) {
	  //R = R * RotationX(INCREMENT);
	  VECTOR4D Axis = {1, 0, 0, 0};
	  R = R * RotationAxis(INCREMENT, Axis);
  }
  if (bRotateY) {
	  //R = R * RotationY(INCREMENT);
	  VECTOR4D Axis = { 0, 1, 0, 0 };
	  R = R * RotationAxis(INCREMENT, Axis);
  }
  if (bRotateZ) {
	  //R = R * RotationZ(INCREMENT);
	  VECTOR4D Axis = { 0, 0, 1, 0 };
	  R = R * RotationAxis(INCREMENT, Axis);
  }
  if (bScaleUp)
    scale += INCREMENT;
  if (bScaleDown)
    scale -= INCREMENT;

  VECTOR4D vec = { mx, my, 0, 0 };
  R = R * RotationAxis(INCREMENT, vec);

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
  case 'd':
  case 'D':
    break;
  case 'c':
  case 'C':
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

void onMouse(int button, int state, int x, int y) {
  mx = -1 + 2 * (float)x / glutGet(GLUT_WINDOW_WIDTH);
  my = 1 - 2 * (float)y / glutGet(GLUT_WINDOW_HEIGHT);
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
    glutCreateWindow("MSC GC 705080 - Exam 02");
  }

  // Initialize viewport and rotation matrix
  Vi = View(Eye, Target, Up);
  R = Identity();

  CImageBMP* pImage = CImageBMP::CreateBitmapFromFile(".\\Cube.bmp", NULL);
  TexDiffuse = pImage->CreateTexture();
  CImageBMP::DestroyBitmap(pImage);

  g_Cube.BuildCube();

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutKeyboardUpFunc(keyUpHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);
  glutPassiveMotionFunc(onPassiveMotion);
  glutMouseFunc(onMouse);

  // Enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}

//EOF
