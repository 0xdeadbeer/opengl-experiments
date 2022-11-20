#version 330

smooth in vec4 the_color;
in vec3 normal_vector;
in vec3 current_position;

out vec4 frag_color; 

void main() {
	vec3 light_source = vec3(1.0f, 0.0f, 0.0f);
	light_source = normalize(light_source.xyz - current_position.xyz);
	vec3 normalized_normal = normalize(normal_vector);
	float lightning = max(dot(normalized_normal, light_source), 0.1f);
	//frag_color = vec4(vec3(frag_color) * lightning, 1.0f);
	frag_color = vec4(the_color.xyz * lightning, 1.0f);
}
