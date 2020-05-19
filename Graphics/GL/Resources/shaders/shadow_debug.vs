#version 430

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 uv;

out vec2 fragment_uv;

void main()
{
    fragment_uv = vec2(uv.x, uv.y);
    vec3 tposition = (position * 0.35) + vec3(0.5, 0.5, 0.0);
    gl_Position = vec4(tposition, 1.0);
}