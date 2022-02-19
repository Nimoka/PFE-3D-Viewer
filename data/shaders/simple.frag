#version 410 core

in vec3 vert_color;

layout(location = 0) out vec3 out_color;

void main() {
	out_color = vec3(vert_color);
}
