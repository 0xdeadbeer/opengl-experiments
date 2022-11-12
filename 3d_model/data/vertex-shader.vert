#version 330 core

layout (location = 0) in vec4 position;

void main() {
	float scale = 0.2;
	gl_Position = vec4(position.x * scale, position.y * scale, position.z * scale, 1.0f);
}
