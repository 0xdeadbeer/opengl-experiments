#ifndef SHADERS_LOADER_H 
#define SHADERS_LOADER_H 

// structs
struct splitted_string {
	size_t size; 
	char *string;
};

struct splitted_strings_arr {
	splitted_string *splitted_strings_arr; 
	int elements; 
};

struct vertex {
	float x; 
	float y; 
	float z; 
	float w; 
}; 

struct split_string_properties {
	int split_start_index; 
	int split_end_index; 
	int found_counter; 
};

/**
Split the string into substrings. 

<Longer description>
Given a string (char array) and a delimiter, split it into multiple
substrings (char arrays) and return a structure containing the substrings.

@data: pointer to the input string
@size: size of the input string 
@delimiter: char that will serve as a delimiter for the split operation 
@free_data: if 1 the function will free the memory for data at the end of the function, 0 for not
@return: structure containing the number of substrings, and a pointer to the array of substrings
*/
splitted_strings_arr split(char *data, size_t size, char delimiter, int free_data);

/**
Size of a file. 

<Longer description>
Return the size of a file located on the computer's filesystem (number of bytes - characters)

@path: path to the file 
@return: number of bytes inside the file 
*/
long int sizeof_file(const char *path); 

/**
Read contents of a file. 

<Warning>
The function assumes the required memory is already allocated to where it will write the contents.

<Longer description>
Write contents of a file into a memory location.

@path: path to the file 
@file_contents: pointer to a memory location where the function will write the file contents.
*/
void read_file(const char *path, char *file_contents); 

/**
Load shader into the program.

<Long description>
The function will write the given shader file, compile the shader, check it, attach it,
link it to the specified program and free/remove all of the junk at the end of the function.

@program: program to link the shader to 
@path: path to the shader file 
@type: shader type
*/
void load_shader(GLuint program, const char *path, GLenum type); 

/**
Load model into the program. 

<Supported file formats>
.obj

<Warning>
This function does not assume the output_buffer has already allocated memory.
Meaning that the function will allocate the required memory by itself. 

<Long description>
Read vertex data from a given file and write it to an output buffer.

@path: path to the model file 
@output_buffer: pointer to the memory where the function will write the vertex data.
*/
float *load_model(const char *path, float *output_buffer); 

#endif
