#version 330 core


in vec4 original_vtx_color;
out vec4 FragColor;
uniform int IDmaterial;


void main()
{
	FragColor = original_vtx_color;
}