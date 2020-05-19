#version 430 core

in vec3 fragment_color;
in vec2 fragment_uv;

out vec4 color;
uniform sampler2D textureSample;

void main()
{
	color = texture(textureSample, fragment_uv) * vec4(fragment_color, 1.0);
}
