#include "Scene16.h"
#include "objects\light.h"
#include "objects\camera.h"
#include "objects\sphere.h"
#include "objects\cube.h"
#include "objects\point_light.h"
#include "objects\directional_light.h"
#include "objects\spotlight_light.h"
#include "objects\model.h"
#include "objects\plane.h"
#include "Math\transform.h"

#define SHADOW_BUFFER_WIDTH	1024
#define SHADOW_BUFFER_HEIGHT 1024

bool Scene16::Initialize()
{

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	PointLight* light = this->CreateObject<PointLight>("point_light");
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light->transform.translation = glm::vec3(2.0f, 2.0f, 3.0f);

	m_depthShader = new Shader();
	m_depthShader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\shadow_depth.vs", GL_VERTEX_SHADER);
	m_depthShader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\shadow_depth.fs", GL_FRAGMENT_SHADER);
	m_depthShader->Link();
	GLuint depthTexture = Material::CreateDepthTexture(SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);

	Model* model = this->CreateObject<Model>("model");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\suzanne.obj");
	model->transform.translation = glm::vec3(0.0f, 0.25f, 0.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_shadow.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_shadow.fs", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.0f);
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.1f);
	model->m_material.shininess = 100.0f;

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\uvgrid.jpg", GL_TEXTURE0);
	model->m_material.AddTexture(depthTexture, GL_TEXTURE1);
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model = this->CreateObject<Plane>("floor");
	((Plane*)model)->Initialize(10.0f, 10.0f, 10.0, 10.0, 5.0f, 5.0f);
	model->transform.matrix = glm::mat4(1.0f);
	model->transform.scale = glm::vec3(8.0f);
	model->transform.translation = glm::vec3(0.0f, -5.0f, 0.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_shadow.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_shadow.fs", GL_FRAGMENT_SHADER);


	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(0.5f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\uvgrid.jpg", GL_TEXTURE0);
	model->m_material.AddTexture(depthTexture, GL_TEXTURE1);
	//model->m_material.SetTextures();
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model = this->CreateObject<Model>("debug");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "meshes\\quad.obj");

	model->m_material.AddTexture(depthTexture, GL_TEXTURE0);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\shadow_depth_debug.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\shadow_depth_debug.fs", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.0f);
	model->m_material.diffuse = glm::vec3(0.8f);
	model->m_material.specular = glm::vec3(0.1f);
	model->m_material.shininess = 100.0f;

	m_depthBuffer = Material::CreateDepthbuffer(depthTexture, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);

	return true;

}

void Scene16::Shutdown()
{
	//this->RemoveAllObjects();
}

void Scene16::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	glm::mat4 mxBias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	PointLight* light = this->GetObject<PointLight>("point_light");
	glm::mat4 mxProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 60.0f);
	glm::mat4 mxLightView = glm::lookAt(light->transform.translation, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxLVP = mxProjection * mxLightView;
	light->transform.translation = light->transform.translation * glm::quat(glm::vec3(0.0f, glm::radians((dt)*45.0f), 0.0f));
	m_depthShader->Use();

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	Model* model = this->GetObject<Model>("model");
	m_time = m_time + dt * 0.25f;
	model->transform.rotation = glm::quat(glm::vec3(0.0f, glm::radians(glm::sin(m_time)* 45.0f), 0.0f));

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

		glm::mat4 mxMLVP = mxBias * mxLVP * model->transform.matrix;
		model->m_shader.SetUniform("light_view_projection_matrix", mxMLVP);
	}

	//m_engine->Get<UI>()->Start();

}

void Scene16::Render()
{
	glViewport(0, 0, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	m_depthShader->Use();
	auto model = GetObject<Model>("model");
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);
	model->m_vertexArrays.Draw();
	model = GetObject<Model>("floor");
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);
	model->m_vertexArrays.Draw();
	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->GetWidth(), m_engine->Get<Renderer>()->GetHeight());
	m_engine->Get<Renderer>()->ClearBuffer();
	glCullFace(GL_BACK);
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}
	m_engine->Get<Renderer>()->SwapBuffer();

}
