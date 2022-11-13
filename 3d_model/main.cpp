#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders-loader.h"

// global variables
GLuint program;
GLuint object_buffer; 
GLuint color_buffer; 

vertex_data data = load_model("./models/object.obj");
color_data color_data = randomize_color(data.len/3);

// setup memory function
void memory_setup() {
	program = glCreateProgram();

	load_shader(program, "./data/vertex-shader.vert", GL_VERTEX_SHADER);
	load_shader(program, "./data/fragment-shader.frag", GL_FRAGMENT_SHADER);

	glGenBuffers(1, &object_buffer); 
	glGenBuffers(1, &color_buffer);

	glBindBuffer(GL_ARRAY_BUFFER, object_buffer); 
	glBufferData(GL_ARRAY_BUFFER, data.len*4*sizeof(float), data.buffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer); 
	glBufferData(GL_ARRAY_BUFFER, color_data.len*4*sizeof(float), color_data.buffer, GL_STATIC_DRAW); 

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

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer); 
	
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, data.len);
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// load the models into memory, setup the functions
	memory_setup();
  glutDisplayFunc(display);

	// enter the event loop
  glutMainLoop();
  return 0;
}
