#include "scene02.h"
#include "Objects/camera.h"

bool Scene02::Initialize()
{
	m_shader = new Shader();
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_uv.vs", GL_VERTEX_SHADER);
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "shaders\\basic_uv.fs", GL_FRAGMENT_SHADER);
	m_shader->Link();
	m_shader->Use();

	// vertex buffer data
	static float cube_vertices[] = {
		// Front
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		// Right
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 // Back
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		  // Left
		  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  -1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		  // Bottom
		  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		   1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		   1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		   // Top
		   -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		   -1.0f,  1.0f, -1.0f, 0.0f, 1.0f
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

	m_vertexArray = new VertexArray(m_engine);
	m_vertexArray->CreateBuffer(5 * sizeof(GLfloat), sizeof(cube_vertices) / (5 * sizeof(GLfloat)), (void*)cube_vertices);
	m_vertexArray->CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_vertices) / sizeof(GLushort), (void*)indices);

	m_vertexArray->SetAttribute(0, 3, (5 * sizeof(GLfloat)), 0);
	m_vertexArray->SetAttribute(1, 2, (5 * sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));

	//texture
	GLuint textureID = Material::LoadTexture(m_engine->Get<FileSystem>()->GetPathname() + "face.png");

	glBindTexture(GL_TEXTURE_2D, textureID);

	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("up", SDL_SCANCODE_UP, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("down", SDL_SCANCODE_DOWN, Input::KEYBOARD);

	//m_mxProjection = glm::ortho(0.0f, (float)m_engine->Get<Renderer>()->GetWidth(), (float)m_engine->Get<Renderer>()->GetHeight(), 0.0f);

	m_mxProjection = glm::perspective(glm::radians(45.0f), (float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight(), 0.01f, 1000.0f);
	m_transform.scale = glm::vec3(20.0f);

	//m_transform.translation = glm::vec3(400.0f, 300.0f, 0.0f);

	m_camera = new Camera(this);

	bool quit = false;
	return true;
}

void Scene02::Shutdown()
{
}

void Scene02::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	if (m_engine->Get<Input>()->GetActionButton("left") == Input::HELD) m_transform.rotation.y -= 10.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("right") == Input::HELD) m_transform.rotation.y += 10.0f *  dt;
	if (m_engine->Get<Input>()->GetActionButton("up") == Input::HELD) m_transform.rotation.x -= 10.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("down") == Input::HELD) m_transform.rotation.x += 10.0f *  dt;

	m_transform.rotation.y += glm::radians(45.0f * dt);

	m_camera->SetView(glm::vec3(0.0f, 0.0f, 80.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_transform.Update();
	glm::mat4 mxMVP = m_mxProjection * m_camera->transform.matrix * m_transform.matrix;

	//m_mxModel = glm::rotate(m_mxModel, 1.0f * dt * m_direction, glm::vec3(0.0f, 0.0f, 1.0f));

	m_shader->SetUniform("model", mxMVP);

}

void Scene02::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	m_vertexArray->Draw(GL_TRIANGLES);

	m_engine->Get<Renderer>()->SwapBuffer();
}
