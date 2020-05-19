#include "scene06.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/directional_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"

bool Scene06::Initialize()
{
	m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f));

	PointLight * light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);
	light->diffuse = glm::vec3(1.0f, 0.0f, 1.0f);
	light->specular = glm::vec3(1.0f);

	DirectionalLight* directional_light = this->CreateObject<DirectionalLight>("directional_light");
	directional_light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	directional_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directional_light->SetDirection(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f));

	Model* model = this->CreateObject<Cube>("model1");
	((Cube*)model)->Initialize(1.0f);
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
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
		
	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	//model2
	model = this->CreateObject<Plane>("model2");
	((Plane*)model)->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	model->transform.translation = glm::vec3(0.0f, -5.0f, 0.0f);
	model->transform.scale = glm::vec3(50.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.fs", GL_FRAGMENT_SHADER);

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

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);

	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	bool quit = false;
	return true;
}

void Scene06::Shutdown()
{
}

void Scene06::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Camera* camera = this->GetObject<Camera>();

	Light* light = this->GetObject<PointLight>();
	light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));
	light->transform.translation = light->transform.rotation * glm::vec3(3.0f, 3.0f, 3.0f);

	if (m_engine->Get<Input>()->GetActionButton("point") == Input::eButtonState::HELD) m_lightIndex = 0;
	if (m_engine->Get<Input>()->GetActionButton("directional") == Input::eButtonState::HELD) m_lightIndex = 1;

	glm::vec4 lightPosition;

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	//update lights
	if (m_lightIndex == 0)
	{
		light = this->GetObject<PointLight>();
		lightPosition = ((PointLight*)light)->GetPositionFromView(camera->transform.matrix);
	}
	else if (m_lightIndex == 1)
	{
		light = this->GetObject<DirectionalLight>();
		lightPosition = ((DirectionalLight*)light)->GetDirectionFromView(camera->transform.matrix);
	}

	//model lights
	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model * model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", lightPosition);
	}
}

void Scene06::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}
