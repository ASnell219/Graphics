#version 430 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_uv;
in vec3 fragment_direction;

uniform samplerCube textureSample;

out vec4 color;

void main()
{
	color = texture(textureSample, fragment_direction);
}