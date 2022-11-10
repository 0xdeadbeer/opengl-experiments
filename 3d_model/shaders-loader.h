#ifndef SHADERS_LOADER_H 
#define SHADERS_LOADER_H 

long int sizeof_file(const char *path); 
void read_file(const char *path, char *file_contents); 
void load_shader(GLuint program, const char *path, GLenum type); 

#endif
