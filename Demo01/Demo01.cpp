//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Demo 01 - First OpenGL program using GLUT and GLEW
// Based on: http://www.lighthouse3d.com/tutorials/glut-tutorial/initialization/
//
// Mario Contreras (705080)
//
//*********************************************************

#include "stdafx.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include "GL\freeglut.h"
#include <math.h>


float time;

void renderScene(void) {
  time += 0.1f;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Do not use. Screen will be blank if used.
  //glClearDepth(-1.0f);

  // Fixed triangle
  //glBegin(GL_TRIANGLES);
  //glVertex3f(-0.5, -0.5, 0.0);
  //glVertex3f(0.5, 0.0, 0.0);
  //glVertex3f(0.0, 0.5, 0.0);
  //glEnd();

  // 2 triangles, one of them has one vertex moving in circles when events are processed
  glBegin(GL_TRIANGLES);
  glColor3d(1, 1, 1);
  glVertex3f((float)(-0.5f * cos(time)), (float)(-0.5f * sin(time)), -1.0f);
  glVertex3f(                      0.5f,                       0.0f,  0.0f);
  glVertex3f(                      0.0f,                       0.5f,  0.0f);

  glColor3f(0, 1, 0);
  glVertex3f(-0.1f, -0.5f, 0.0f);
  glVertex3f( 0.5f,  0.0f, 0.0f);
  glVertex3f( 0.0f,  0.5f, 0.0f);
  glEnd();

  glutSwapBuffers();
}

void keyHandler(unsigned char key, int x, int y) {
  if(key == 27) {
    glutLeaveMainLoop();
  }
}

int main(int argc, char **argv) {

  // init GLUT and create Window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(480, 480);
  glutCreateWindow("MSC GC 705080 - My first OpenGL program");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyHandler);

  // enter GLUT event processing cycle
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();

  return 0;
}

//EOF
