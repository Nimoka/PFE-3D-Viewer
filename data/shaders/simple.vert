#version 330 core

layout (location = 0) in vec3 vtx_position;
layout (location = 1) in vec3 vtx_color;
layout (location = 3) in vec3 vtx_normal;


out vec3 original_vtx_color

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vert_pos_view;
out vec3 vert_normal_view;



void main()
{   
    vert_pos_view  = view* model * vec4(vtx_position,1.0f);
	gl_Position = projection * vert_pos_view;
    vert_normal_view = normalize(normal_matrix * vtx_normal);
	original_vtx_color = vec4(vtx_color, 1.0f);
}