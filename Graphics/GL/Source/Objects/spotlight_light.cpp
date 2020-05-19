#include "spotlight_light.h"
#include "core/scene.h"
#include "cube.h"

SpotLight::SpotLight(Scene* scene, const std::string& name) : Light(scene, name)
{
	m_model = new Cube(scene);
	m_model->Initialize(0.1f);

	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_color.vs", GL_VERTEX_SHADER);
	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	m_model->m_shader.Link();
}

void SpotLight::Update()
{
	transform.Update();
	m_model->transform = transform;
	m_model->Update();
	m_model->m_shader.SetUniform("material.diffuse", diffuse);
}

void SpotLight::Draw()
{
	m_model->Draw();
}

glm::vec4 SpotLight::GetPositionFromView(const glm::mat4& mxView)
{
	return mxView * transform.matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void SpotLight::SetDirection(const glm::vec3 & position, const glm::vec3 & target, const glm::vec3 & up)
{
	transform.matrix = glm::inverse(glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f)));
	transform.rotation = glm::quat_cast(transform.matrix);
	transform.translation = position;
}

glm::vec3 SpotLight::GetDirectionFromView(const glm::mat4& mxView)
{
	return glm::mat3(mxView * transform.matrix) * glm::vec3(0.0f, 0.0f, -1.0f);
}

