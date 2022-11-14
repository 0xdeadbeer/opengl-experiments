#version 330 core

//smooth in vec4 the_color; 
smooth in vec3 normal_vector;

out vec4 frag_color; 

void main() {

	vec3 normal_vector_position = normal_vector.xyz + gl_FragCoord.xyz;
	vec3 light_source = vec3(1.0f, 0.0f, 0.0f);
	light_source = normalize(light_source);
	vec3 normalized_normal = normalize(normal_vector_position);

	float lightning = dot(normalized_normal, light_source);

	// vec4 color = the_color; 
	frag_color = vec4(lightning, lightning, lightning, 1.0f);
}
