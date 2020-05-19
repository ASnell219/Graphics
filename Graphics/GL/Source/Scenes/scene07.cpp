#include "scene07.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/spotlight_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Math/transform.h"

bool Scene07::Initialize()
{
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f));

	SpotLight* spot_light = this->CreateObject<SpotLight>("spot_light");
	spot_light->diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	spot_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spot_light->cutoff = glm::radians(45.0f);
	spot_light->exponent = 10.0f;

	spot_light->transform.translation = glm::vec3(-10.0f, 10.0f, 10.0f);
	spot_light->SetDirection(spot_light->transform.translation, glm::vec3(0.0f, 0.0f, 0.0f));


	Model* model = this->CreateObject<Cube>("model1");
	((Cube*)model)->Initialize(1.0f);
	model->transform.scale = glm::vec3(5.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	
	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-diffuse.png", GL_TEXTURE0);
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", spot_light->diffuse);
	model->m_shader.SetUniform("light.specular", spot_light->specular);
	model->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	model->m_shader.SetUniform("light.exponent", spot_light->exponent);
		
	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	//model2
	model = this->CreateObject<Plane>("model2");
	((Plane*)model)->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	model->transform.translation = glm::vec3(0.0f, -5.0f, 0.0f);
	model->transform.scale = glm::vec3(50.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);

	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\cobble-diffuse.tga", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", spot_light->diffuse);
	model->m_shader.SetUniform("light.specular", spot_light->specular);

	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	bool quit = false;
	return true;
}

void Scene07::Shutdown()
{
}

void Scene07::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Camera* camera = this->GetObject<Camera>();

	SpotLight* light = this->GetObject<SpotLight>();
	//light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));

	glm::vec4 lightPosition;
	glm::vec3 lightDirection;

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	//update lights
	light = this->GetObject<SpotLight>();
	lightPosition = ((SpotLight*)light)->GetPositionFromView(camera->transform.matrix);
	lightDirection = ((SpotLight*)light)->GetDirectionFromView(camera->transform.matrix);

	//model lights
	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model* model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", lightPosition);
		model->m_shader.SetUniform("light.direction", lightDirection);
		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);
		model->m_shader.SetUniform("light.cutoff", light->cutoff);
		model->m_shader.SetUniform("light.exponent", light->exponent);
	}

	m_engine->Get<UI>()->Start();
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Diffuse", (float*)&light->diffuse);
	ImGui::ColorEdit3("Specular", (float*)&light->specular);
	ImGui::End();

	ImGui::Begin("Transform");
	Model * model = this->GetObject<Model>("model1");
	Transform::Edit(&model->transform);
	ImGui::End();

}

void Scene07::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<UI>()->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}
