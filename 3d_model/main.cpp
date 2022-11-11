#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders-loader.h"

// global variables
GLuint program;
GLuint object_buffer; 

// tmp data 
float vertex_positions[] = {
	1.0f, 1.0f, 0.0f, 1.0f, 
	0.0f, 0.0f, 0.0f, 1.0f, 
	-1.0f, 1.0f, 0.0f, 1.0f,
};

// setup memory function
void memory_setup() {
	program = glCreateProgram();

	load_shader(program, "./data/vertex-shader.vert", GL_VERTEX_SHADER);
	load_shader(program, "./data/fragment-shader.frag", GL_FRAGMENT_SHADER);

	glGenBuffers(1, &object_buffer); 
	glBindBuffer(GL_ARRAY_BUFFER, object_buffer); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
}

// update output - display function
void display()
{
	glUseProgram(program);
  glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, object_buffer); 

	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

// main function
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("3D Models challenge");
	
	GLenum glew_err = glewInit(); 
	if (GLEW_OK != glew_err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_err));
		return 1;
	}

	// load the models into memory, setup the functions
	memory_setup();
  glutDisplayFunc(display);

	// enter the event loop
  glutMainLoop();
  return 0;
}
