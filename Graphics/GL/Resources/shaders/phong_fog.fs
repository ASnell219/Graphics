#version 430 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 fragment_uv;

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

struct Fog
{
	float distance_min;
	float distance_max;
	vec3 color;
};

uniform Fog fog;
uniform light_s		light;
uniform material_s	material;
layout (binding = 0) uniform sampler2D textureSample;

out vec4 color;

void main()
{
	vec3 position_to_light = normalize(vec3(light.position) - fragment_position);

	// ambient
	vec3 ambient = material.ambient;

	// diffuse
	float diffuse_intensity = max(dot(position_to_light, fragment_normal), 0.0);
	vec3 diffuse = material.diffuse * light.diffuse * diffuse_intensity;

	// specular
	vec3 specular = vec3(0.0);
	if (diffuse_intensity > 0.0)
	{
		vec3 position_to_view = normalize(-fragment_position.xyz);
		vec3 reflect_light = reflect(-position_to_light, fragment_normal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = light.specular * material.specular * specular_intensity;
	}
	//fog
	float distance = abs(fragment_position.z);
	float fog_intensity = clamp((distance - fog.distance_min) / (fog.distance_max - fog.distance_min), 0.0f, 1.0f);

	vec4 fragment_color = texture(textureSample, fragment_uv) * vec4(ambient + diffuse, 1.0) + vec4(specular, 1.0);
	color = mix(fragment_color, vec4(fog.color, 1.0), fog_intensity);
}