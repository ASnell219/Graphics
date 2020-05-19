#version 430

layout (location = 0) in vec3 position;

uniform mat4 light_view_projection_matrix;
uniform mat4 model_matrix;

void main()
{    
	gl_Position = light_view_projection_matrix * model_matrix * vec4(position, 1.0);
}