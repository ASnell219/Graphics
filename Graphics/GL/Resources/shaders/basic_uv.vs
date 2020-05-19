#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 fragment_uv;
uniform mat4 model;

void main()
{
	fragment_uv = uv;
	gl_Position = model * vec4(position.x, position.y, position.z, 1.0);
}