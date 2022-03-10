#version 410 core

uniform vec3 lights_dir_direction[NB_DIR_LIGHTS];
uniform vec3 lights_dir_intensity[NB_DIR_LIGHTS];

//uniform vec3 lights_pt_position[NB_PT_LIGHTS];
//uniform vec3 lights_pt_intensity[NB_PT_LIGHTS];

uniform vec3 ambient_color;
uniform mat3 normal_matrix;

in vec4 vert_position;
in vec3 vert_color;
in vec3 vert_normal;

layout(location = 0) out vec3 out_color;

void main() {
	out_color = ambient_color * vert_color;
	for (int i = 0; i < NB_DIR_LIGHTS; i++) {
		out_color += (vert_color * lights_dir_intensity[i])
				* max(dot(vert_normal, normalize(lights_dir_direction[i]
						* normal_matrix)), 0);
	}
}
