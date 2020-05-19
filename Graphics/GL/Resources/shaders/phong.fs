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

uniform light_s light;
uniform light_s lights[5];
uniform material_s	material;
layout (binding = 0) uniform sampler2D textureSample;

out vec4 color;

void phong(int light_index, vec3 position, vec3 normal, out vec3 diffuse, out vec3 specular)
{
	light_s light = lights[light_index];
	
	vec3 position_to_light = normalize(vec3(light.position) - fragment_position);
	
	// diffuse
	float diffuse_intensity = max(dot(position_to_light, fragment_normal), 0.0);
	diffuse = material.diffuse * light.diffuse * diffuse_intensity;

	// specular
	specular = vec3(0.0);
	if (diffuse_intensity > 0.0)
	{
		vec3 position_to_view = normalize(-fragment_position.xyz);
		vec3 reflect_light = reflect(-position_to_light, fragment_normal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = light.specular * material.specular * specular_intensity;
	}
}

void main()
{
	vec3 final_color = material.ambient;

	for(int i = 0; i < 5; i++)
	{
		vec3 diffuse;
		vec3 specular;
		phong(i, fragment_position, fragment_normal, diffuse,  specular);
		final_color += (diffuse + specular);
	}

	color = texture(textureSample, fragment_uv) * vec4(final_color, 1.0);
}