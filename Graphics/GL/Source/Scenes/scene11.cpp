#include "Scene11.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Math/transform.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/color_space.hpp>

bool Scene11::Initialize()
{
	Camera* camera = this->CreateObject<Camera>(); 
	camera->Initialize(); 
	camera->SetView(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->type = Camera::eType::EDITOR;
	
	PointLight* light = this->CreateObject<PointLight>("point_light"); 
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f); 
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f); 
	light->transform.translation = glm::vec3(0.0f, 1.0f, 2.0f); 
	
	Model* model = this->CreateObject<Model>("model1"); 
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\ogre.obj"); 
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_nm.vs", GL_VERTEX_SHADER); 
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_nm.fs", GL_FRAGMENT_SHADER);

	model->m_shader.Link(); 
	model->m_shader.Use(); 

	model->m_material.ambient = glm::vec3(0.2f); 
	model->m_material.diffuse = glm::vec3(1.0f); 
	model->m_material.specular = glm::vec3(1.0f); 
	model->m_material.shininess = 100.0f; 

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\ogre_diffuse.bmp", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\ogre_normal.bmp", GL_TEXTURE1);
	//model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse); 
	model->m_shader.SetUniform("material.specular", model->m_material.specular); 
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess); 

	return true;
}

void Scene11::Shutdown()
{
}

void Scene11::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Model* model = this->GetObject<Model>("model1");
	m_time = m_time + dt * 0.25f;
	model->transform.rotation = glm::quat(glm::vec3(0.0f, glm::radians(glm::sin(m_time) * 25.0f), 0.0f));
	
	PointLight* light = this->GetObject<PointLight>("point_light"); 
	light->transform.translation = light->transform.translation * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(25.0f * dt)));

	std::vector<Object*> objects = this->GetObjects<Object>(); 
	for (Object* object : objects) 
	{ 
		object->Update(); 
	}

	Camera* camera = this->GetObject<Camera>(); 
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix); 
	
	std::vector<Model*> models = this->GetObjects<Model>(); 
	for (Model* model : models) 
	{
		model->m_shader.Use(); 

		model->m_shader.SetUniform("light.position", lightPosition); 
		model->m_shader.SetUniform("light_position", lightPosition); 

		model->m_shader.SetUniform("light.diffuse", light->diffuse); 
		model->m_shader.SetUniform("light.specular", light->specular); 

		model->m_shader.SetUniform("material.ambient", model->m_material.ambient); 
		model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse); 
		model->m_shader.SetUniform("material.specular", model->m_material.specular); 
		model->m_shader.SetUniform("material.shininess", model->m_material.shininess); 
	}
	
	//m_engine->Get<UI>()->Start();
}

void Scene11::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>(); 
	for (Renderable* renderable : renderables) 
	{ 
		renderable->Draw(); 
	}

	//m_engine->Get<UI>()->Draw(); 

	m_engine->Get<Renderer>()->SwapBuffer();
}
