#version 330 core

layout (location = 0) in vec3 vtx_position;
layout (location = 1) in vec3 vtx_color;


out vec3 original_vtx_color

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vtx_position, 1.0f);
	original_vtx_color = vec4(vtx_color, 1.0f);
}