#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec2 fragment_uv;

uniform mat4		model_view_matrix;
uniform mat4		model_view_projection_matrix;
uniform mat3		normal_matrix;

void main()
{
	fragment_position = vec3(model_view_matrix * vec4(position, 1.0));
	fragment_normal = normalize(normal_matrix * normal);
	fragment_uv = uv;
	
	gl_Position = model_view_projection_matrix * vec4(position.x, position.y, position.z, 1.0);
}