#version 430 core

layout (location = 0) in vec3 position;

out vec3 fragment_color;

struct material_s
{
	vec3 ambient;
	vec3 diffuse;
};

uniform material_s material;
uniform mat4 model_view_projection_matrix;

void main()
{
	fragment_color = material.diffuse;
	gl_Position = model_view_projection_matrix * vec4(position.x, position.y, position.z, 1.0);
}