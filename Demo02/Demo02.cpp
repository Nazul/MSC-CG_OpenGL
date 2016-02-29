//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 02 - First 3D object (and custom Matrix & Vector library)
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include "Matrix4D.h"
#include <math.h>
#include "GL\\freeglut.h"
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

#define REFRESH_MILISECS 100
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

bool rotateX, rotateY, rotateZ = false;

VECTOR4D Triangle[3] = {
  {  0,  1, 0, 1 },
  {  1, -1, 0, 1 },
  { -1, -1, 0, 1 }
};

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

float angle = 0.0f;

MATRIX4D R;

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int sx = glutGet(GLUT_WINDOW_WIDTH);
  int sy = glutGet(GLUT_WINDOW_HEIGHT);
  MATRIX4D SAspect = Scaling((float)sy / sx, 1, 1);
  VECTOR4D Target = { 0.0f, 0.0f, 0.0f, 1.0f };
  VECTOR4D Eye = { 0.2f, 0.2f, 0.2f, 1.0f };
  VECTOR4D Up = { 0.0f, 0.0f, 1.0f, 0.0f };
  MATRIX4D Vi = View(Eye, Target, Up);

  glBegin(GL_TRIANGLES);

  //for (int i = 0; i < sizeof(Triangle) / sizeof(VECTOR4D); i++) {
  //  glColor4f(Colors[i].r, Colors[i].g, Colors[i].b, Colors[i].a);
  //  VECTOR4D V = SAspect * Vi * R * Scaling(0.25f, 0.25f, 0.25f) * Triangle[i];
  //  glVertex4f(V.x, V.y, V.z, V.w);
  //}

  for (int i = 0; i < sizeof(PyramidIndices) / sizeof(int); i += 3) {
    for (int j = 0; j < 3; j++) {
      glColor4f(Colors[PyramidIndices[i + j]].r, Colors[PyramidIndices[i + j]].g, Colors[PyramidIndices[i + j]].b, Colors[PyramidIndices[i + j]].a);
      VECTOR4D V = SAspect * Vi * R * Scaling(0.5f, 0.5f, 0.5f) * Pyramid[PyramidIndices[i + j]];
      glVertex4f(V.x, V.y, V.z, V.w);
    }
  }

  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);
  angle = 0.03f;

  //R = Identity();
  if (rotateX)
    R = R * RotationX(angle);
  if (rotateY)
    R = R * RotationY(angle);
  if (rotateZ)
    R = R * RotationZ(angle);
  glutPostRedisplay();
}

void keyHandler(unsigned char key, int x, int y) {
#ifdef _DEBUG
  std::cout << key << " pressed" << std::endl;
#endif
  if (key == 27)
    glutLeaveMainLoop();
  if (key == 'x' || key == 'X')
    rotateX = !rotateX;
  if (key == 'y' || key == 'Y')
    rotateY = !rotateY;
  if (key == 'z' || key == 'Z')
    rotateZ = !rotateZ;
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  bool gameMode = false;
  int startX = 200, startY = 200;

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
    glutCreateWindow("MSC GC 705080 - Demo 2 Matrices, Vectors");
  }

  R = Identity();

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  // Enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}

//EOF
