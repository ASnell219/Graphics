#include "directional_light.h"
#include "core/scene.h"
#include "cube.h"

DirectionalLight::DirectionalLight(Scene* scene, const std::string& name) : Light(scene, name)
{
	m_model = new Cube(scene);
	m_model->Initialize(0.1f);

	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_color.vs", GL_VERTEX_SHADER);
	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_color.fs", GL_FRAGMENT_SHADER);
	m_model->m_shader.Link();
}

void DirectionalLight::Update()
{
	transform.Update();
	m_model->transform = transform;
	m_model->Update();
	m_model->m_shader.SetUniform("material.diffuse", diffuse);
}

void DirectionalLight::Draw()
{
	m_model->Draw();
}

void DirectionalLight::SetDirection(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	transform.matrix = glm::inverse(glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f)));
	transform.rotation = glm::quat_cast(transform.matrix);
	transform.translation = position;
}

glm::vec4 DirectionalLight::GetDirectionFromView(const glm::mat4& mxView)
{
	glm::vec3 direction = glm::mat3(mxView * transform.matrix) * glm::vec3(0.0f, 0.0f, 1.0f);
	return glm::vec4(direction, 0.0f);
}
