#version 330 core

layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 color; 
layout (location = 2) in vec3 normal;

//smooth out vec4 the_color; 
smooth out vec3 normal_vector;

uniform float offset; 

float frustum_scale = 1.0f;

void main() {

	vec4 camera_pos = position;
	camera_pos.z -= 3.0f;
	 camera_pos.xy += offset;
	// vec3 normal_calc = normal.xyz + camera_pos.xyz;

	vec4 clip_pos;
	clip_pos.xy = camera_pos.xy * frustum_scale;
	
	clip_pos.z = camera_pos.z + 1.0f;
	clip_pos.w = -camera_pos.z;

	gl_Position = clip_pos;

	//the_color = color;
	normal_vector = normal;
}
