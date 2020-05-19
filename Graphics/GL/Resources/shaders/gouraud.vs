#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec2 fragment_uv;
out vec3 fragment_color;

struct material_s
{
    vec3 ambient;
    vec3 diffuse;
};

struct light_s
{
    vec4 position;
    vec3 diffuse;
};

uniform light_s        light;
uniform material_s    material;
uniform mat4        model_view_matrix;
uniform mat4        model_view_projection_matrix;
uniform mat3        normal_matrix;

void main()
{
    vec3 transform_normal = normalize(normal_matrix * normal);
    vec4 transform_postion = model_view_matrix * vec4(position, 1.0);
    vec3 position_to_light = normalize(vec3(light.position - transform_postion));

    vec3 ambient = material.ambient;
    vec3 diffuse = material.diffuse * light.diffuse * max(dot(position_to_light, transform_normal), 0.0);
    fragment_color = ambient + diffuse;
    gl_Position = model_view_projection_matrix * vec4(position.x, position.y, position.z, 1.0);
}