#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "general-functions.h"

// global variables
int x_screen = 900; 
int y_screen = 900; 
float x_limit = x_screen / 30.0f; 
float y_limit = y_screen / 30.0f; 

GLuint program, vao;
GLuint player_vertex_buffer, player_normal_buffer,
        block_vertex_buffer, block_normal_buffer,
        bullet_vertex_buffer, bullet_normal_buffer; 
GLuint color_unif, offset_unif; 

// models (vertecies + normals)
vertex_data player_model = load_model("./models/player.obj");
vertex_data block_model = load_model("./models/block.obj");
vertex_data bullet_model = load_model("./models/bullet.obj");

// colors
float player_color[] = { 0.0f, 1.0f, 1.0f, 1.0f }; 
float block_color[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
float bullet_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };

// offsets
float player_offset[] = { 0.0f, -30.0f, -35.0f, 0.0f }; 
float block_offset[] = { 0.0f, 20.0f, -35.0f, 0.0f }; 
float bullet_offset[] = { -9.0f, -28.0f, -35.0f, 0.0f };

// forces
float player_force[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
float block_force[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
float bullet_force[] = { 5.0f, 5.0f, 0.0f, 0.0f }; 

// keyboard input forces 
float player_movement_force = 1.0f; 

// force counters
int bullet_force_counter = 1;  

// visible 
bool player_visible = 0; 
bool block_visible = 0; 
bool bullet_visible = 0; 


/**
collision data 

1-2: x start - x end 
3-4: y start - y end 
5-6: z start - z end 
7-8: w start - w end 
*/

float player_collision_bound[] = {
  -1.0f, 1.0f, 
  -1.0f, 1.0f, 
  -1.0f, 1.0f,
  -1.0f, 1.0f, 
}; 
float block_collision_bound[] = {
  -2.5f, 2.5f, 
  -1.0f, 1.0f, 
  -1.0f, 1.0f,
  -1.0f, 1.0f 
}; 
float bullet_collision_bound[] = {
  -0.5f, 0.5f, 
  -0.5f, 0.5f, 
  -0.5f, 0.5f, 
  -0.5f, 0.5f
}; 

// consts
float force_scaler = 5000.0f; 

// apply forces to all the objects
void apply_forces() {
  for (int i = 0; i < 4; i++) {
    player_offset[i] += player_force[i] / force_scaler; 
    block_offset[i] += block_force[i] / force_scaler; 
    bullet_offset[i] += bullet_force[i] / force_scaler; 
  }
}

// rotate the force of the bullet 
void rotate_forces() {
  switch (bullet_force_counter) {
    case 1: 
      bullet_force[0] = -bullet_force[0]; 
      break; 
    case 2: 
      bullet_force[1] = -bullet_force[1]; 
      break; 
    case 3: 
      bullet_force[0] = -bullet_force[0]; 
      break; 
    case 4: 
      bullet_force[1] = -bullet_force[1]; 
      break; 
  }

  if (bullet_force_counter < 4) {
    bullet_force_counter++; 
    return; 
  }
  
  bullet_force_counter = 1; 
}

// setup memory function
void memory_setup() {
	program = glCreateProgram();

	load_shader(program, "./data/vertex-shader.vert", GL_VERTEX_SHADER);
	load_shader(program, "./data/fragment-shader.frag", GL_FRAGMENT_SHADER);

  glGenBuffers(1, &player_vertex_buffer); 
  glGenBuffers(1, &player_normal_buffer); 

  glGenBuffers(1, &block_vertex_buffer); 
  glGenBuffers(1, &block_normal_buffer); 

  glGenBuffers(1, &bullet_vertex_buffer); 
  glGenBuffers(1, &bullet_normal_buffer); 

  // get uniform locations
  color_unif = glGetUniformLocation(program, "color");
  offset_unif = glGetUniformLocation(program, "offset"); 

  // load into buffers
  load_gpu_data(player_vertex_buffer, GL_ARRAY_BUFFER, player_model.len*4*sizeof(float), player_model.vertex_buffer, GL_DYNAMIC_DRAW);
  load_gpu_data(player_normal_buffer, GL_ARRAY_BUFFER, player_model.len*3*sizeof(float), player_model.normals_buffer, GL_DYNAMIC_DRAW); 

  load_gpu_data(block_vertex_buffer, GL_ARRAY_BUFFER, block_model.len*4*sizeof(float), block_model.vertex_buffer, GL_DYNAMIC_DRAW);
  load_gpu_data(block_normal_buffer, GL_ARRAY_BUFFER, block_model.len*3*sizeof(float), block_model.normals_buffer, GL_DYNAMIC_DRAW); 

  load_gpu_data(bullet_vertex_buffer, GL_ARRAY_BUFFER, bullet_model.len*4*sizeof(float), bullet_model.vertex_buffer, GL_DYNAMIC_DRAW);
  load_gpu_data(bullet_normal_buffer, GL_ARRAY_BUFFER, bullet_model.len*3*sizeof(float), bullet_model.normals_buffer, GL_DYNAMIC_DRAW); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
}

// update output - display function
void display()
{
 
  // object updates
  if (1 == collision(bullet_collision_bound, block_collision_bound, bullet_offset, block_offset) || 
      1 == collision(bullet_collision_bound, player_collision_bound, bullet_offset, player_offset) || 
      1 == touching_borders(bullet_collision_bound, bullet_offset, x_limit, y_limit)) 
      rotate_forces(); 

  apply_forces(); 

	glUseProgram(program);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnableVertexAttribArray(0); 
  glEnableVertexAttribArray(1); 

  // draw the player
	glBindBuffer(GL_ARRAY_BUFFER, player_vertex_buffer); 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, player_normal_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniform4fv(color_unif, 1, player_color);
  glUniform4fv(offset_unif, 1, player_offset); 

	glDrawArrays(GL_TRIANGLES, 0, player_model.len);

  // draw the block
  glBindBuffer(GL_ARRAY_BUFFER, block_vertex_buffer); 
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); 
  
  glBindBuffer(GL_ARRAY_BUFFER, block_normal_buffer); 
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glUniform4fv(color_unif, 1, block_color); 
  glUniform4fv(offset_unif, 1, block_offset); 
  
  glDrawArrays(GL_TRIANGLES, 0, block_model.len);
  
  // draw the bullet 
  glBindBuffer(GL_ARRAY_BUFFER, bullet_vertex_buffer); 
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); 
  
  glBindBuffer(GL_ARRAY_BUFFER, bullet_normal_buffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glUniform4fv(color_unif, 1, bullet_color); 
  glUniform4fv(offset_unif, 1, bullet_offset);

  glDrawArrays(GL_TRIANGLES, 0, bullet_model.len);

	glUseProgram(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

// keyboard input function 
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'a': 
      player_offset[0] -= player_movement_force; 
      break; 
    case 'd': 
      player_offset[0] += player_movement_force; 
      break; 
    case 's':
      player_offset[1] -= player_movement_force; 
      break; 
    case 'w': 
      player_offset[1] += player_movement_force; 
      break; 
  }
}

// main function
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(x_screen, y_screen);
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

	// load the models into memory
	memory_setup();

  // setup the functions 
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard); 

	// enter the event loop
  glutMainLoop();
  return 0;
}
