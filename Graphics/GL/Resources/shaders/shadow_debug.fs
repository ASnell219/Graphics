#version 430

in vec2 fragment_uv;

layout (binding = 0) uniform sampler2D depthSample;

out vec4 color;

void main()
{
	float depth = texture(depthSample, fragment_uv).x;
	color = vec4(vec3(depth), 1.0);
}