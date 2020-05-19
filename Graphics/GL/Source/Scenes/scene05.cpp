#include "scene05.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"

bool Scene05::Initialize()
{
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f));
	//m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);

	PointLight * light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->specular = glm::vec3(1.0f);

	Model* model = this->CreateObject<Sphere>();
	((Sphere*)(model))->Initialize(1.0f, 20, 20);
	model->transform.scale = glm::vec3(5.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);
	
	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;


	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-diffuse.png", GL_TEXTURE0);

	//model->m_shader.SetUniform("fog.distance_min", 20.0f);
	//model->m_shader.SetUniform("fog.distance_max", 50.0f);
	//model->m_shader.SetUniform("fog.color", glm::vec3(0.85f, 0.85f, 0.85f));
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
		
	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	//model2
	model = this->CreateObject<Cube>();
	((Cube*)(model))->Initialize(1.0f);
	model->transform.translation = glm::vec3(10.0f, 5.0f, 5.0f);
	model->transform.scale = glm::vec3(5.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);

	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-diffuse.png", GL_TEXTURE0);

	//model->m_shader.SetUniform("fog.distance_min", 20.0f);
	//model->m_shader.SetUniform("fog.distance_max", 50.0f);
	//model->m_shader.SetUniform("fog.color", glm::vec3(0.85f, 0.85f, 0.85f));

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);

	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	bool quit = false;
	return true;
}

void Scene05::Shutdown()
{
}

void Scene05::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}
	
	Camera* camera = this->GetObject<Camera>();

	PointLight* light = this->GetObject<PointLight>();
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);

	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model * model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", lightPosition);
	}
}

void Scene05::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}
