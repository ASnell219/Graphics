#version 430 core

subroutine vec3 reflection_model_t();
subroutine uniform reflection_model_t reflection_model;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 fragment_direction;

uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat4 model_view_projection_matrix;
uniform mat3 normal_matrix;
uniform vec3 camera_position;

subroutine(reflection_model_t)
vec3 skybox()
{
	return position;
}

subroutine(reflection_model_t)
vec3 reflection()
{
	// get world position
	// get world normal
	// get world view =  camera position - world position

	// return reflection value
		// reflect -world view, world normal

    vec3 world_position = vec3(model_matrix * vec4(position,1.0) );
    vec3 world_normal = normalize(normal_matrix * normal);
    vec3 world_view = normalize(camera_position - world_position);

    return reflect(-world_view, world_normal);
}

subroutine(reflection_model_t)
vec3 refraction()
{
	// get world position
	// get world normal
	// get world view = camera position - world position - normalize
	
	// return refraction value
		// refract -world view, world normal, ratio (1.0/refraction index)
	vec3 world_position = vec3(model_matrix * vec4(position,1.0) );
    vec3 world_normal = normalize(normal_matrix * normal);
    vec3 world_view = normalize(camera_position - world_position);

    return refract(-world_view, world_normal, (1.0/2.42));
}

void main()
{
    fragment_direction = reflection_model();
	
	gl_Position = model_view_projection_matrix * vec4(position.x, position.y, position.z, 1.0);
}