#include "scene08.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Math/transform.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/color_space.hpp>

#define NUM_LIGHTS 5
#define GLM_ENABLE_EXPERIMENTAL

bool Scene08::Initialize()
{
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f));

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		PointLight* light = this->CreateObject<PointLight>("point_light");
		glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));
		light->diffuse = color;
		light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 position = glm::sphericalRand(5.0f);
		light->transform.translation = position;
	}

	auto lights = GetObjects<PointLight>();

	Model* model = this->CreateObject<Model>("model1");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\doggo.obj");
	model->transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);
	
	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.1f);
	model->m_material.diffuse = glm::vec3(0.5f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 150.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\uvgrid.jpg", GL_TEXTURE0);
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	//model2
	model = this->CreateObject<Plane>("model2");
	((Plane*)model)->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	model->transform.translation = glm::vec3(0.0f, -5.0f, 0.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);

	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.1f);
	model->m_material.diffuse = glm::vec3(0.5f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 150.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\cobble-diffuse.tga", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	bool quit = false;
	return true;
}

void Scene08::Shutdown()
{
}

void Scene08::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Camera* camera = this->GetObject<Camera>();

	glm::vec4 lightPosition;

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	auto lights = GetObjects<Light>(); 
	for (size_t i = 0; i < lights.size(); i++) 
	{
		// calculate light position
		lightPosition = ((PointLight*)lights[i])->GetPositionFromView(camera->transform.matrix);
		auto models = GetObjects<Model>();

		for (auto model : models)
		{
			// set uniform for model shader
			// make sure you call Use() on the model->shader so that it is set
			model->m_shader.Use();
			// create name of uniform like above sprintf_s but it’ll be position
			char uniformName[32];
			sprintf_s(uniformName, "lights[%d].position", i);
			model->m_shader.SetUniform(uniformName, lightPosition);
			sprintf_s(uniformName, "lights[%d].diffuse", i);
			model->m_shader.SetUniform(uniformName, lights[i]->diffuse);
			sprintf_s(uniformName, "lights[%d].specular", i);
			model->m_shader.SetUniform(uniformName, lights[i]->specular);
		}
	}
}

void Scene08::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}
