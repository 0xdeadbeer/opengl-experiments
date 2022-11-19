#version 330

smooth in vec4 the_color; 

out vec4 frag_color; 

void main() {
	frag_color = the_color;
}
