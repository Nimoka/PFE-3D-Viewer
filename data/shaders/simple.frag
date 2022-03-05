#version 410 core

in vec4 vert_position;
in vec3 vert_color;
in vec3 vert_normal;
in vec3 vert_normal_raw;

layout(location = 0) out vec3 out_color;

void main() {
	out_color = vec3(vert_color);
}
