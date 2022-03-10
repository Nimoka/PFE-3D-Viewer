#version 410 core

in vec4 vert_position;
in vec3 vert_color;
in vec3 vert_normal;
in vec3 vert_normal_raw;

uniform usamplerBuffer face_material;

layout(location = 0) out vec3 out_color;

void main() {
	out_color = vec3(vert_color);

	// uint material = uint(texelFetch(face_material, gl_PrimitiveID).r);
	// out_color = vec3(material);
}
