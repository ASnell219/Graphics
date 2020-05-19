#include "scene04.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"

#define MULTI_TEXTURE
//#define SPECULAR_MAP

static float cube_vertices[] = {
	// Front
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	// Right
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 // Back
	 -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	  // Left
	  -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	  -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	  // Bottom
	  -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	   // Top
	   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};

static GLushort indices[] =
{
	 0,  1,  2,  0,  2,  3,
	 4,  5,  6,  4,  6,  7,
	 8,  9, 10,  8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
};

bool Scene04::Initialize()
{
	Camera * camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));
	//m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);

	Sphere* model = this->CreateObject<Sphere>();
	model->Initialize(1.0f, 20, 20);
	model->transform.scale = glm::vec3(5.0f);


#ifdef SPECULAR_MAP
model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_specular_map.fs", GL_FRAGMENT_SHADER);
#elif defined MULTI_TEXTURE
model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi_texture.fs", GL_FRAGMENT_SHADER);
#endif

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	//shader
	//model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi_texture.vs", GL_VERTEX_SHADER);
	//model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\phong_multi_texture.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();


	//m_material = new Material();

	model->m_material.ambient = glm::vec3(0.2f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;

#ifdef SPECULAR_MAP
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-specular.png", GL_TEXTURE1);
#elif defined MULTI_TEXTURE
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "\\Textures\\moss.png", GL_TEXTURE1);
#endif

	model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	model->m_shader.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		

	bool quit = false;
	return true;
}

void Scene04::Shutdown()
{
}

void Scene04::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Sphere* model = this->GetObject<Sphere>();

	model->transform.rotation = model->transform.rotation * glm::angleAxis(glm::radians(45.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));

	Camera * camera = this->GetObject<Camera>();
	camera->Update();

	m_uvOffset.y = m_uvOffset.y + (0.1f * dt); 
	model->m_shader.SetUniform("uv_offset", m_uvOffset);

	//update light
	glm::vec4 lightPosition = camera->transform.matrix * glm::vec4(15.0f, 15.0f, 50.0f, 1.0f);
	model->m_shader.SetUniform("light.position", lightPosition);

	glm::mat3 mxNormal = glm::mat3(camera->transform.matrix * model->transform.matrix);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	model->m_shader.SetUniform("normal_matrix", mxNormal);

	//update model view
	glm::mat4 mxMV = camera->transform.matrix * model->transform.matrix;
	model->m_shader.SetUniform("model_view_matrix", mxMV);

	//update mvp
	model->transform.Update();
	glm::mat4 mxMVP = camera->projection * camera->transform.matrix * model->transform.matrix;
	model->m_shader.SetUniform("model_view_projection_matrix", mxMVP);

}

void Scene04::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	Sphere* model = this->GetObject<Sphere>();
	model->Draw();
	//m_vertexArray->Draw(GL_TRIANGLES);

	m_engine->Get<Renderer>()->SwapBuffer();
}
