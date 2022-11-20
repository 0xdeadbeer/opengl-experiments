#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "general-functions.h"

// global variables
GLuint program, vao;
GLuint object_buffer, normal_buffer; 
GLuint color_unif, offset_unif; 

// models (vertecies + normals)
vertex_data player_model = load_model("./models/player.obj");
vertex_data block_model = load_model("./models/block.obj");
vertex_data bullet_model = load_model("./models/bullet.obj");

// colors
GLfloat player_color[] = { 0.0f, 1.0f, 1.0f, 1.0f }; 
GLfloat block_color[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
GLfloat bullet_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };

// offsets
GLfloat player_offset[] = { 0.0f, -30.0f, -35.0f, 0.0f }; 
GLfloat block_offset[] = { 0.0f, 30.0f, -35.0f, 0.0f }; 
GLfloat bullet_offset[] = { 0.0f, 0.0f, -5.0f, 0.0f };

// scales 

// rotations

// switch vbo data
void switch_vbo(GLuint buffer, GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) {
  glBindBuffer(target, buffer); 
  glBufferData(target, size, data, usage); 
}

// setup memory function
void memory_setup() {
	program = glCreateProgram();

	load_shader(program, "./data/vertex-shader.vert", GL_VERTEX_SHADER);
	load_shader(program, "./data/fragment-shader.frag", GL_FRAGMENT_SHADER);

	glGenBuffers(1, &object_buffer); 
	glGenBuffers(1, &normal_buffer);

  // get uniform locations
  color_unif = glGetUniformLocation(program, "color");
  offset_unif = glGetUniformLocation(program, "offset"); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
}

// update output - display function
void display()
{
	glUseProgram(program);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnableVertexAttribArray(0); 
  glEnableVertexAttribArray(1); 

  switch_vbo(object_buffer, GL_ARRAY_BUFFER, player_model.len*4*sizeof(float), player_model.vertex_buffer, GL_STATIC_DRAW);
  switch_vbo(normal_buffer, GL_ARRAY_BUFFER, player_model.len*3*sizeof(float), player_model.normals_buffer, GL_STATIC_DRAW); 

	glBindBuffer(GL_ARRAY_BUFFER, object_buffer); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniform4fv(color_unif, 1, player_color);
  glUniform4fv(offset_unif, 1, player_offset); 

	glDrawArrays(GL_TRIANGLES, 0, player_model.len);


  switch_vbo(object_buffer, GL_ARRAY_BUFFER, player_model.len*4*sizeof(float), block_model.vertex_buffer, GL_STATIC_DRAW); 
  switch_vbo(normal_buffer, GL_ARRAY_BUFFER, player_model.len*3*sizeof(float), block_model.normals_buffer, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, object_buffer); 
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); 
  
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer); 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniform4fv(color_unif, 1, block_color); 
  glUniform4fv(offset_unif, 1, block_offset); 
  
  glDrawArrays(GL_TRIANGLES, 0, block_model.len);

	glUseProgram(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

// main function
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(1000, 1000);
  glutCreateWindow("Blocks game");
	
	GLenum glew_err = glewInit(); 
	if (GLEW_OK != glew_err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_err));
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);
	
	// debug 
	GLenum error = glGetError(); 
	printf("DEBUG: %d\n", error);

	// load the models into memory, setup the functions
	memory_setup();
  glutDisplayFunc(display);

	// enter the event loop
  glutMainLoop();
  return 0;
}
