#version 430 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_uv;
in mat3 fragment_tbn;
in vec3 fragment_position_to_light;
in vec3 fragment_position_to_view;

struct material_s
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct light_s
{
	vec4 position;
	vec3 diffuse;
	vec3 specular;
};

uniform light_s light;
uniform material_s	material;

layout (binding = 0) uniform sampler2D textureSample;
layout (binding = 1) uniform sampler2D normalSample;

out vec4 color;

void phong(vec3 position, vec3 normal, out vec3 diffuse, out vec3 specular)
{
	vec3 position_to_light = fragment_position_to_light;
	
	// diffuse
	float diffuse_intensity = max(dot(position_to_light, normal), 0.0);
	diffuse = material.diffuse * light.diffuse * diffuse_intensity;

	// specular
	specular = vec3(0.0);
	if (diffuse_intensity > 0.0)
	{
		vec3 position_to_view = fragment_position_to_view;//normalize(-fragment_position.xyz);
		vec3 reflect_light = reflect(-position_to_light, normal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = light.specular * material.specular * specular_intensity;
	}
}

void main()
{
	vec3 ambient = material.ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 normal = texture(normalSample, fragment_uv).rgb;
	normal = normalize((normal * 2.0) - 1.0);
	//normal = normalize(normal * fragment_tbn);

	phong(fragment_position, normal, diffuse,  specular);

	vec4 textureColor = texture(textureSample, fragment_uv);
	color = textureColor * vec4(ambient + diffuse, 1.0) + vec4(specular, 1.0);
}