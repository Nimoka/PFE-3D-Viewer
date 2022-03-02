#version 410 core

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

in vec3 vtx_position;
in vec3 vtx_color;
in vec3 vtx_normal;

out vec3 vert_color;
out vec3 vert_normal;

void main() {
	gl_Position = projection_matrix * view_matrix * vec4(vtx_position, 1.);
	vert_color = vtx_color;
	vert_normal = vtx_normal;
}