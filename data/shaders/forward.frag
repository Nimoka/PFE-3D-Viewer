#version 410 core

uniform vec3 light_direction;
uniform vec3 light_intensity;
uniform mat3 normal_matrix;

in vec4 vert_position;
in vec3 vert_color;
in vec3 vert_normal;

layout(location = 0) out vec3 out_color;

void main() {
	out_color = (vert_color * light_intensity)
			* max(dot(vert_normal,
					normalize(light_direction * normal_matrix)), 0);
}
