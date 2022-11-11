#ifndef SHADERS_LOADER_H 
#define SHADERS_LOADER_H 

// structs
struct splitted_string {
	size_t size; 
	char *string;
};

struct split_string_properties{
	int split_start_index; 
	int split_end_index; 
	int found_counter; 
};

int split(char *data, splitted_string *output_data, size_t size, char delimiter);
long int sizeof_file(const char *path); 
void read_file(const char *path, char *file_contents); 
void load_shader(GLuint program, const char *path, GLenum type); 
void load_model(const char *path, float *output_buffer); 

#endif
