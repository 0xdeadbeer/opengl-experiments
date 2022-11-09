#include <GL/freeglut_std.h>
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

void displayMe(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
  glEnd();
  glFlush();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hello world from Badprog.com :D");
  glutDisplayFunc(displayMe);
  glutMainLoop();
  return 0;
}

void glutPassiveMotionFunc(int x, int y) {
  printf("sup?\n");
}
