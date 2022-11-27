#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// STRING MANIPULATION STRUCTURES
struct splitted_string {
  size_t size; 
  char *string; 
}; 

struct splitted_strings_arr {
  splitted_string *splitted_strings_arr; 
  int elements; 
}; 

struct split_string_properties {
  int split_start_index; 
  int split_end_index; 
  int found_counter; 
}; 

// ABSTRACT STRUCTURES
struct vertex {
  float x; 
  float y; 
  float z; 
  float w; 
}; 

struct normal {
  float x; 
  float y; 
  float z; 
}; 

// DATA STRUCTURES
struct vertex_data {
  float *vertex_buffer; 
  size_t len; 

  float *normals_buffer; 
}; 

struct color_data {
  float *buffer; 
  size_t len; 
}; 

struct normal_data {
  float *buffer; 
  size_t len; 
}; 

// GENERAL DATA STRUCTURES
struct linked_list {
  void *value; 
  size_t len; 
  linked_list *next; 

  int null = 0; 
}; 

struct mutual_linked_list {
  mutual_linked_list *prev; 
  void *value; 
  size_t len;
  mutual_linked_list *next; 
  
  int null = 0; 
}; 

#endif 
