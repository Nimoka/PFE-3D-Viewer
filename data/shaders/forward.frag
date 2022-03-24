#version 410 core

@define_macros

uniform vec3 lights_dir_direction[NB_DIR_LIGHTS];
uniform vec3 lights_dir_intensity[NB_DIR_LIGHTS];

uniform vec3 lights_pt_position[NB_PT_LIGHTS];
uniform vec3 lights_pt_intensity[NB_PT_LIGHTS];

uniform vec3 ambient_color;
uniform mat3 normal_matrix;

in vec4 vert_position;
in vec3 vert_color;
in vec3 vert_normal;

uniform usamplerBuffer face_material;

layout(location = 0) out vec3 out_color;

@define_materials

void main() {
	out_color = ambient_color * vert_color;

@call_materials for (int i = 0; i < NB_DIR_LIGHTS; i++) { out_color += (@mat * lights_dir_intensity[i]) * max(dot(vert_normal, normalize(lights_dir_direction[i] * normal_matrix)), 0); } for (int i = 0; i < NB_PT_LIGHTS; i++) { float distance = length(lights_pt_position[i] - vert_position.xyz); float attenuation =  1. / (1. + .09 * distance + .032 * distance * distance); vec3 lightDir = normalize((lights_pt_position[i] - vert_position.xyz)); out_color += @mat * lights_pt_intensity[i] * max(dot(vert_normal, lightDir * normal_matrix), 0) * attenuation; }
}
