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

float angle = 0.0;
float dx, dy, dz = 0.0;

VECTOR4D baseTriangle[3];
VECTOR4D tranTriangle[3];
MATRIX4D rx;
MATRIX4D ry;
MATRIX4D rz;
MATRIX4D rt;

bool rotateX, rotateY, rotateZ = false;
bool traslate = false;

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex4f(tranTriangle[0].x, tranTriangle[0].y, tranTriangle[0].z, tranTriangle[0].w);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex4f(tranTriangle[1].x, tranTriangle[1].y, tranTriangle[1].z, tranTriangle[1].w);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex4f(tranTriangle[2].x, tranTriangle[2].y, tranTriangle[2].z, tranTriangle[2].w);
  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);
  angle += 1.0f;
  if (angle >= 360) {
    angle = 0.0f;
  }
  dx += 0.01f;
  if (dx > 1.0f)
    dx = 0.0f;

  if (rotateX) {
    rx = RotationX(angle);
    for (int i = 0; i < 3; i++) {
      tranTriangle[i].x = baseTriangle[i].x * rx.m00 + baseTriangle[i].y * rx.m01 + baseTriangle[i].z * rx.m02 + baseTriangle[i].w * rx.m03;
      tranTriangle[i].y = baseTriangle[i].x * rx.m10 + baseTriangle[i].y * rx.m11 + baseTriangle[i].z * rx.m12 + baseTriangle[i].w * rx.m13;
      tranTriangle[i].z = baseTriangle[i].x * rx.m20 + baseTriangle[i].y * rx.m21 + baseTriangle[i].z * rx.m22 + baseTriangle[i].w * rx.m23;
      tranTriangle[i].w = baseTriangle[i].x * rx.m30 + baseTriangle[i].y * rx.m31 + baseTriangle[i].z * rx.m32 + baseTriangle[i].w * rx.m33;
    }
  }
  if (rotateY) {
    ry = RotationY(angle);
    for (int i = 0; i < 3; i++) {
      tranTriangle[i].x = baseTriangle[i].x * ry.m00 + baseTriangle[i].y * ry.m01 + baseTriangle[i].z * ry.m02 + baseTriangle[i].w * ry.m03;
      tranTriangle[i].y = baseTriangle[i].x * ry.m10 + baseTriangle[i].y * ry.m11 + baseTriangle[i].z * ry.m12 + baseTriangle[i].w * ry.m13;
      tranTriangle[i].z = baseTriangle[i].x * ry.m20 + baseTriangle[i].y * ry.m21 + baseTriangle[i].z * ry.m22 + baseTriangle[i].w * ry.m23;
      tranTriangle[i].w = baseTriangle[i].x * ry.m30 + baseTriangle[i].y * ry.m31 + baseTriangle[i].z * ry.m32 + baseTriangle[i].w * ry.m33;
    }
  }
  if (rotateZ) {
    rz = RotationZ(angle);
    for (int i = 0; i < 3; i++) {
      tranTriangle[i].x = baseTriangle[i].x * rz.m00 + baseTriangle[i].y * rz.m01 + baseTriangle[i].z * rz.m02 + baseTriangle[i].w * rz.m03;
      tranTriangle[i].y = baseTriangle[i].x * rz.m10 + baseTriangle[i].y * rz.m11 + baseTriangle[i].z * rz.m12 + baseTriangle[i].w * rz.m13;
      tranTriangle[i].z = baseTriangle[i].x * rz.m20 + baseTriangle[i].y * rz.m21 + baseTriangle[i].z * rz.m22 + baseTriangle[i].w * rz.m23;
      tranTriangle[i].w = baseTriangle[i].x * rz.m30 + baseTriangle[i].y * rz.m31 + baseTriangle[i].z * rz.m32 + baseTriangle[i].w * rz.m33;
    }
  }
  if (traslate) {
    rt = Translation(dx, dx, dx);
    for (int i = 0; i < 3; i++) {
      tranTriangle[i].x = baseTriangle[i].x * rt.m00 + baseTriangle[i].y * rt.m01 + baseTriangle[i].z * rt.m02 + baseTriangle[i].w * rt.m03;
      tranTriangle[i].y = baseTriangle[i].x * rt.m10 + baseTriangle[i].y * rt.m11 + baseTriangle[i].z * rt.m12 + baseTriangle[i].w * rt.m13;
      tranTriangle[i].z = baseTriangle[i].x * rt.m20 + baseTriangle[i].y * rt.m21 + baseTriangle[i].z * rt.m22 + baseTriangle[i].w * rt.m23;
      tranTriangle[i].w = baseTriangle[i].x * rt.m30 + baseTriangle[i].y * rt.m31 + baseTriangle[i].z * rt.m32 + baseTriangle[i].w * rt.m33;
    }
  }

  glutPostRedisplay();
}

void keyHandler(unsigned char key, int x, int y) {
#ifdef _DEBUG
  std::cout << key << " pressed" << std::endl;
#endif
  if (key == 27) {
    glutLeaveMainLoop();
  }
  if (key == 'x' || key == 'X')
    rotateX = !rotateX;
  if (key == 'y' || key == 'Y')
    rotateY = !rotateY;
  if (key == 'z' || key == 'Z')
    rotateZ = !rotateZ;
  if (key == 't' || key == 'T')
    traslate = !traslate;
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  bool gameMode = false;
  int startX = 300, startY = 300;

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

  // Define base triangle
  baseTriangle[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
  baseTriangle[1] = {  0.5f,  0.0f, 0.0f, 1.0f };
  baseTriangle[2] = {  0.0f,  0.5f, 0.0f, 1.0f };
  baseTriangle[3] = {  0.0f,  0.0f, 0.0f, 1.0f };

  // Initialize transformed triangle
  tranTriangle[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
  tranTriangle[1] = {  0.5f,  0.0f, 0.0f, 1.0f };
  tranTriangle[2] = {  0.0f,  0.5f, 0.0f, 1.0f };
  tranTriangle[3] = {  0.0f,  0.0f, 0.0f, 1.0f };

  // Initialize rotation matrices
  rx = Identity();
  ry = Identity();
  rz = Identity();

  // Initialize traslation matrix
  rt = Identity();

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  // Enter GLUT event processing cycle
  glutMainLoop();

  return 0;
}

//EOF
