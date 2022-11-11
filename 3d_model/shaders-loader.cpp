#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders-loader.h"

long int sizeof_file(const char *path) {
	FILE *file_pointer = fopen(path, "r"); 

	if (NULL == file_pointer) {
		fprintf(stderr, "File can't be opened\n");
		return -1;
	}

	fseek(file_pointer, 0L, SEEK_END); 

	return (long int) ftell(file_pointer) / sizeof(char); 
}

void read_file(const char *path, char *file_contents) {
	FILE *file_pointer = fopen(path, "r");

	if (NULL == file_pointer) {
		fprintf(stderr, "File can't be opened\n"); 
		return;
	}

	int character_index = -1;
	while(1) {
		character_index++;

		char curr_char = fgetc(file_pointer);
		if (EOF == curr_char) {
			break; 
		}

		file_contents[character_index] = curr_char; 
	}
	
	fclose(file_pointer); 
}

void check_shader(GLuint shader, char *path) {
	GLint result = GL_FALSE; 
	int log_length; 

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result); 
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length); 

	char *shader_error = (char *) malloc((log_length > 1) ? log_length : 1);
	glGetShaderInfoLog(shader, log_length, NULL, &shader_error[0]);

	if (0x0 != shader_error[0]) {
		fprintf(stderr, "Shader '%s': %s\n", path, shader_error);
	}
}

void load_shader(GLuint program, const char *path, GLenum type) {
	GLuint shader = glCreateShader(type); 

	long int shader_size = sizeof_file(path);
	if (0 == shader_size) {
		fprintf(stdout, "info: '%s' file shader size is 0\n", path); 
		return; 
	}
	char* file_contents = (char *) malloc(shader_size); 

	read_file(path, file_contents);  

	// compile shader
	glShaderSource(shader, 1, &file_contents, NULL);
	glCompileShader(shader);

	// check the shader
	check_shader(shader, (char *) path);

	// attach shader & link shader
	glAttachShader(program, shader);
	glLinkProgram(program);

	// free memory and junk
	glDeleteShader(shader);
	free(file_contents);
}
