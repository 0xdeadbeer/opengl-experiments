#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders-loader.h"

splitted_strings_arr split(char *data, splitted_string *output_data, size_t size, char delimiter) {
	split_string_properties properties = {0,0,0};
	output_data = (splitted_string *) malloc(0);

	for (int i = 0; i < size; i++) {
		char current_char = *(data + i); 
		if (delimiter != current_char) continue; 

		properties.found_counter++; 
		output_data = (splitted_string *) realloc(output_data, (size_t) ((properties.found_counter+1) * sizeof(splitted_string)));
		if (NULL == output_data) {
			fprintf(stderr, "error: failed to reallocate memory");
			return splitted_strings_arr { NULL, -1 }; 
		}
		properties.split_end_index = i; 
		
		size_t split_size = properties.split_end_index - properties.split_start_index; 
		char *split_string = (char *) malloc(split_size); 
		
		strncpy(split_string, (const char*) (data + properties.split_start_index), split_size); 	
		
		*(output_data+properties.found_counter) = { split_size, split_string }; 
		if (size-1 >= i+1) properties.split_start_index = i+1; 
	}

	return splitted_strings_arr { output_data, properties.found_counter }; 
}

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

void load_model(const char *path, float *output_buffer) {
	long int src_size = sizeof_file(path);
	if (0 == src_size) {
		fprintf(stdout, "info: model file is empty!");
		return;
	}
	char *model_src = (char *) malloc(src_size); 

	read_file(path, model_src);
	splitted_string *lines; 
	splitted_strings_arr test = split(model_src, lines, (size_t) src_size, '\n');
	printf("size of array -> %d\n", test.elements);

	// print the whole file model 
	for (int i = 1; i <= test.elements; i++) {
		//printf("Size of the string: %d\n", (test.splitted_strings_arr+i)->size);
		printf("String itself: %s\n", (test.splitted_strings_arr+i)->string);
	}

}
