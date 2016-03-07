//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 03 - Perspective and view
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include "Matrix4D.h"
#include <math.h>
#include "GL\freeglut.h"
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

#define REFRESH_MILISECS 100
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480
#define INCREMENT 0.01f

bool rotateX, rotateY, rotateZ, scaleUp, scaleDown, forward, backward, left, right, up, down = false;

VECTOR4D Pyramid[4] = {
  {  0,  1, 0, 1 },
  {  1, -1, 0, 1 },
  { -1, -1, 0, 1 },
  {  0,  0, 1, 1 }
};

VECTOR4D Colors[4] = {
  { 1, 0, 0, 1 },
  { 0, 1, 0, 1 },
  { 0, 0, 1, 1 },
  { 1, 1, 1, 1 }
};

int PyramidIndices[] = {
  0, 1, 2,
  0, 2, 3,
  2, 1, 3,
  0, 3, 1
};

MATRIX4D T;
MATRIX4D R;
MATRIX4D Vi;


void renderScene(void) {
  int sx = glutGet(GLUT_WINDOW_WIDTH);
  int sy = glutGet(GLUT_WINDOW_HEIGHT);
  MATRIX4D SAspect = Scaling((float)sy / sx, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < sizeof(PyramidIndices) / sizeof(int); i += 3) {
    for (int j = 0; j < 3; j++) {
      VECTOR4D C = Colors[PyramidIndices[i + j]];
      VECTOR4D V = SAspect * T * Pyramid[PyramidIndices[i + j]];
      glColor4f(C.r, C.g, C.b, C.a);
      glVertex4f(V.x, V.y, V.z, V.w);
    }
  }
  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  static float scale = 1.0f;
  VECTOR4D Speed = { 0.02f, 0.02f, 0.02f, 0.0f };
  MATRIX4D P = PerspectiveWidthHeightRH(0.5f, 0.5f, 1.0f, 10.0f);
  MATRIX4D InvV = FastInverse(Vi);
  VECTOR4D XDir = { InvV.m00, InvV.m10, InvV.m20, 0.0f };
  VECTOR4D YDir = { InvV.m01, InvV.m11, InvV.m21, 0.0f };
  VECTOR4D ZDir = { InvV.m02, InvV.m12, InvV.m22, 0.0f };
  VECTOR4D EyePos = { InvV.m03, InvV.m13, InvV.m23, 1.0f };

  if (left)
    EyePos = EyePos + XDir * Speed;
  if (right)
    EyePos = EyePos - XDir * Speed;
  if (down)
    EyePos = EyePos + YDir * Speed;
  if (up)
    EyePos = EyePos - YDir * Speed;
  if (backward)
    EyePos = EyePos + ZDir * Speed;
  if (forward)
    EyePos = EyePos - ZDir * Speed;
  InvV.m03 = EyePos.x;
  InvV.m13 = EyePos.y;
  InvV.m23 = EyePos.z;
  Vi = FastInverse(InvV);

  if (rotateX)
    R = R * RotationX(INCREMENT);
  if (rotateY)
    R = R * RotationY(INCREMENT);
  if (rotateZ)
    R = R * RotationZ(INCREMENT);
  if (scaleUp)
    scale += INCREMENT;
  if (scaleDown)
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
    scaleUp = !scaleUp;
    scaleDown = false;
    break;
  case '-':
    scaleDown = !scaleDown;
    scaleUp = false;
    break;
  case '2':
    down = true;
    break;
  case '4':
    left = true;
    break;
  case '6':
    right = true;
    break;
  case '7':
    backward = true;
    break;
  case '8':
    up = true;
    break;
  case '9':
    forward = true;
    break;
  case 'X':
  case 'x':
    rotateX = !rotateX;
    break;
  case 'Y':
  case 'y':
    rotateY = !rotateY;
    break;
  case 'Z':
  case 'z':
    rotateZ = !rotateZ;
    break;
  }
  glutPostRedisplay();
}

void keyUpHandler(unsigned char key, int x, int y) {
#ifdef _DEBUG
  std::cout << key << " released" << std::endl;
#endif
  switch (key) {
  case '2':
    down = false;
    break;
  case '4':
    left = false;
    break;
  case '6':
    right = false;
    break;
  case '7':
    backward = false;
    break;
  case '8':
    up = false;
    break;
  case '9':
    forward = false;
    break;
  }
  glutPostRedisplay();
}


int main(int argc, char **argv) {
  bool gameMode = false;
  int startX = 200, startY = 200;
  VECTOR4D Target = { 0.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D Eye = { 3.0f, 3.0f, 3.0f, 1.0f };
  VECTOR4D Up = { 0.0f, 0.0f, 1.0f, 0.0f };

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
    glutCreateWindow("MSC GC 705080 - Demo 3 (Perspective and view)");
  }

  Vi = View(Eye, Target, Up);
  R = Identity();

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutKeyboardUpFunc(keyUpHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  // Enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}

//EOF
