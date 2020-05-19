#include "Scene10.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/point_light.h"
#include "Objects/cube.h"
#include "Objects/plane.h"
#include "Math/transform.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/color_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL

bool Scene10::Initialize()
{
	Camera * camera = this->CreateObject<Camera>("camera");
	camera->Initialize();
	camera->type = Camera::eType::EDITOR;
	camera->SetView(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f));

	camera = this->CreateObject<Camera>("camera_rtt");
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->type = Camera::eType::LOOK_AT;
	camera->SetProjection(45.0f, 1.0f, 0.1f, 100.0f);

	PointLight* light = this->CreateObject<PointLight>("point_light");
	light->diffuse = glm::vec3(1.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light->transform.translation = glm::vec3(10.0f);

	Model* model = this->CreateObject<Model>("ogre");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\ogre.obj");
	model->transform.scale = glm::vec3(2.0f);
	model->m_cameraName = "camera_rtt";

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);

	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.1f);
	model->m_material.diffuse = glm::vec3(0.5f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 150.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\ogre_diffuse.bmp", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("lights[0].position", light->transform.translation);
	model->m_shader.SetUniform("lights[0].diffuse", light->diffuse);
	model->m_shader.SetUniform("lights[0].specular", light->specular);

	model = this->CreateObject<Model>("cube");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\cube.obj");
	model->transform.scale = glm::vec3(7.0f);
	model->m_cameraName = "camera";

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong.fs", GL_FRAGMENT_SHADER);
	
	//shader
	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.1f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 150.0f;

	GLuint textureID = Material::CreateTexture(512, 512);
	model->m_material.AddTexture(textureID, GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("lights[0].position", light->transform.translation);
	model->m_shader.SetUniform("lights[0].diffuse", light->diffuse);
	model->m_shader.SetUniform("lights[0].specular", light->specular);

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	GLuint depthBufferID;
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	bool quit = false;
	return true;
}

void Scene10::Shutdown()
{
}

void Scene10::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	
	PointLight* light = this->GetObject<PointLight>();

	Model* model = this->GetObject<Model>("ogre");

	Camera* camera = GetObject<Camera>("camera_rtt");
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);
	model->m_shader.Use();
	model->m_shader.SetUniform("lights[0].position", lightPosition);

	model = this->GetObject<Model>("cube");
	camera = GetObject<Camera>("camera");
	lightPosition = light->GetPositionFromView(camera->transform.matrix);
	model->m_shader.Use();
	model->m_shader.SetUniform("lights[0].position", lightPosition);

	model = this->GetObject<Model>("ogre");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(25.0f * dt), 0.0f));
	model = this->GetObject<Model>("cube");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(glm::radians(15.0f * dt), glm::radians(15.0f * dt), 0.0f));

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}
}

void Scene10::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	glViewport(0, 0, 512, 512);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Model* model = this->GetObject<Model>("ogre");
	model->Draw();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->GetWidth(), m_engine->Get<Renderer>()->GetHeight());
	m_engine->Get<Renderer>()->ClearBuffer();

	model = this->GetObject<Model>("cube");
	model->Draw();

	//m_engine->Get<UI>()->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();

	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
