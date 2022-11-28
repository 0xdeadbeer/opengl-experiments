#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform vec4 color;
uniform vec4 offset; 
uniform vec2 instance_offsets[30]; 

smooth out vec4 the_color;
out vec3 normal_vector;
out vec3 current_position;

float frustum_scale = 1.0f;
float near = 0.1f; 
float far = 50.0f; 

void main() {
	vec4 camera_pos = position;
    camera_pos += offset; 
    camera_pos.xy += instance_offsets[gl_InstanceID]; 
	
	vec3 normal_calc = normalize(normal.xyz);

	vec4 clip_pos;
	clip_pos.xy = camera_pos.xy * frustum_scale;
	clip_pos.z = camera_pos.z * (near + far) / (near - far); 
	clip_pos.z += 2 * near * far / (near - far);
	clip_pos.w = -camera_pos.z;

	gl_Position = clip_pos;
	current_position = camera_pos.xyz;
	normal_vector = normal_calc;
  the_color = color;
}