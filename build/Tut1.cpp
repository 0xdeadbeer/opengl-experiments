#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <glload/gl_3_2_comp.h>
#include <GL/freeglut.h>
#include <math.h>
#include "../framework/framework.h" 
#include <cstring>

GLuint theProgram;

void InitializeProgram()
{
	std::vector<GLuint> shaderList;

  shaderList.push_back(Framework::LoadShader(GL_VERTEX_SHADER, "../Tut 01 Hello Triangle/data/vertex-shader.vert"));
	shaderList.push_back(Framework::LoadShader(GL_FRAGMENT_SHADER, "../Tut 01 Hello Triangle/data/frag-shader.frag"));

	theProgram = Framework::CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

float vertexPositions[] = {
  0.0f, 1.0f, 0.0f, 2.0f,
  -1.0f, -1.0f, 0.0f, 2.0f,
  1.0f, -1.0f, 0.0f, 2.0f,

  -1.0f, -1.0f, 0.0f, 2.0f, 
  0.5f, -0.5f, 0.0f, 2.0f, 
  0.0f, 1.0f, 0.0f, 2.0,

  1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.5f,

  0.5f, 0.5f, 0.5f, 0.5f, 
  0.0f, 0.0f, 0.0f, 0.0f, 
  1.0f, 1.0f, 1.0f, 1.0f,
};

// movement and position 
float obj_x_position = 0; 
float obj_y_position = 0; 
float obj_x_position_updater = 0.1; 
float obj_y_position_updater = 0.1;

// rotation
int point_attr = 4; 

int x_position = 0; 
int y_position = 1; 
int z_position = 2; 

int points = sizeof(vertexPositions) / sizeof(float) / point_attr;
int colorDataIndex = 4 * point_attr * 6; 

GLuint positionBufferObject;
GLuint vao;

float theta = 0.0003; 
float phi = 0.0004; 

void InitializeVertexBuffer()
{
  float theta_rotation_matrix[] = {
    cos(theta), 0, -sin(theta),
    0,          1, 0,
    sin(theta), 0, cos(theta),
  };  

  float phi_rotation_matrix[] = {
    cos(phi),   sin(phi), 0, 
    -sin(phi),  cos(phi), 0,
    0,          0,        1,
  };

  // rotate the points using the defined rotation matrix 
  for (int i = 0; i < points; i++) {
    int x = (i * point_attr) + x_position; 
    int y = (i * point_attr) + y_position; 
    int z = (i * point_attr) + z_position; 

    // fetch the values 
    float fx = vertexPositions[x]; 
    float fy = vertexPositions[y]; 
    float fz = vertexPositions[z]; 

    // rotate via the theta angle 
    // float new_x = (fx * theta_rotation_matrix[0]) + (fy * theta_rotation_matrix[1]) + (fz * theta_rotation_matrix[2]); 
    // float new_y = (fx * theta_rotation_matrix[3]) + (fy * theta_rotation_matrix[4]) + (fz * theta_rotation_matrix[5]); 
    // float new_z = (fx * theta_rotation_matrix[6]) + (fy * theta_rotation_matrix[7]) + (fz * theta_rotation_matrix[8]); 

    // rotate via the phi angle 
    // new_x = (new_x * phi_rotation_matrix[0]) + (new_y * phi_rotation_matrix[1]) + (new_z * phi_rotation_matrix[2]); 
    // new_y = (new_x * phi_rotation_matrix[3]) + (new_y * phi_rotation_matrix[4]) + (new_z * phi_rotation_matrix[5]); 
    // new_z = (new_x * phi_rotation_matrix[6]) + (new_y * phi_rotation_matrix[7]) + (new_z * phi_rotation_matrix[8]); 

    // update/write to the array
    vertexPositions[x] = fx;  
    vertexPositions[y] = fy;
    vertexPositions[z] = fz;
  }

  // float updatedVertexPositions[] = vertexPositions; 
  const int sizeOfData = sizeof(vertexPositions) / sizeof(float); 
  float updatedVertexPositions[sizeOfData] ;

  memcpy(updatedVertexPositions, vertexPositions, sizeof(vertexPositions));

  for (int i = 0; i < points; i++) {
    int x = (i * point_attr) + x_position; 
    int y = (i * point_attr) + y_position; 
    int z = (i * point_attr) + z_position; 

    float fx = updatedVertexPositions[x]; 
    float fy = updatedVertexPositions[y]; 
    float fz = updatedVertexPositions[z]; 

    fx += obj_x_position; 
    fy += obj_y_position;

    updatedVertexPositions[x] = fx; 
    updatedVertexPositions[y] = fy; 
    updatedVertexPositions[z] = fz; 
  }

	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(updatedVertexPositions), updatedVertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
  InitializeVertexBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

	glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1); 

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) colorDataIndex); 

	glDrawArrays(GL_TRIANGLES, 0, points);

	glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1); 

	glUseProgram(0);

  glutSwapBuffers();
  glutPostRedisplay();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:  // user pressing ESC 
      glutLeaveMainLoop(); 
      break;
    case 97:  // user pressing A  
      obj_x_position -= obj_x_position_updater; 
      break; 
    case 100: // user pressing D  
      obj_x_position += obj_x_position_updater; 
      break; 
    case 119: // user pressing W  
      obj_y_position += obj_y_position_updater; 
      break; 
    case 115: // user pressing S 
      obj_y_position -= obj_y_position_updater; 
      break;  
  }
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}
