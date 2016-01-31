//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Homework 01 - Rotate a triangle
//
// Mario Contreras (705080)
//
//*********************************************************

#include "stdafx.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include <math.h>
#ifdef _DEBUG
#include <iostream>
#endif

#define REFRESH_MILISECS 100
#define PI 3.14159265358979f
#define A 0
#define B 1
#define C 2
#define X 0
#define Y 1
#define Z 2

float angle = 0.0;

float baseVertices[3][3] = {
  { -0.5f, -0.5f, 0.0f },
  { 0.5f,  0.0f, 0.0f },
  { 0.0f,  0.5f, 0.0f }
};
float resultVertices[3][3] = {
  { -0.5f, -0.5f, 0.0f },
  { 0.5f,  0.0f, 0.0f },
  { 0.0f,  0.5f, 0.0f }
};
float rotation[2][2] = {
  { 1.0f, 0.0f },
  { 0.0f, 1.0f }
};


void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(resultVertices[A][X], resultVertices[A][Y], resultVertices[A][Z]);
  glVertex3f(resultVertices[B][X], resultVertices[B][Y], resultVertices[B][Z]);
  glVertex3f(resultVertices[C][X], resultVertices[C][Y], resultVertices[C][Z]);
  glEnd();

  glutSwapBuffers();
}

void animateScene(int value) {
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);
  angle += 1.f;
  if (angle >= 360) {
    angle = 0.0f;
  }

  rotation[0][0] =  cos(angle * PI / 180.0f);
  rotation[0][1] = -sin(angle * PI / 180.0f);
  rotation[1][0] =  sin(angle * PI / 180.0f);
  rotation[1][1] =  cos(angle * PI / 180.0f);

  for (int i = 0; i < 3; i++) {
    resultVertices[i][X] = baseVertices[i][X] * rotation[0][0] + baseVertices[i][Y] * rotation[0][1];
    resultVertices[i][Y] = baseVertices[i][X] * rotation[1][0] + baseVertices[i][Y] * rotation[1][1];
  }

#ifdef _DEBUG
  std::cout << std::fixed << "Angle: " << angle << ". Rotation: [{" << rotation[0][0] << ", " << rotation[0][1] << "}, {" << rotation[1][0] << ", " << rotation[1][1] << "}]" << std::endl;
  std::cout << std::fixed << "Vertex A: [{" << resultVertices[A][X] << ", " << resultVertices[A][Y] << ", " << resultVertices[A][Z] << "}]" << std::endl;
  std::cout << std::fixed << "Vertex B: [{" << resultVertices[B][X] << ", " << resultVertices[B][Y] << ", " << resultVertices[B][Z] << "}]" << std::endl;
  std::cout << std::fixed << "Vertex C: [{" << resultVertices[C][X] << ", " << resultVertices[C][Y] << ", " << resultVertices[C][Z] << "}]" << std::endl;
#endif

  glutPostRedisplay();
}

int main(int argc, char **argv) {
  // Init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(480, 480);
  glutCreateWindow("MSC GC 705080 - Homework 01");

  // Register callbacks
  glutDisplayFunc(renderScene);
  glutTimerFunc(REFRESH_MILISECS, animateScene, 0);

  // Enter GLUT event processing cycle
  glutMainLoop();

  return 0;
}

//EOF
