#include "renderer.h"

bool Renderer::Initialize()
{
	m_width = 800;
	m_height = 600;

	// create window
	m_window = SDL_CreateWindow("OpenGL", 100, 100, m_width, m_height, SDL_WINDOW_OPENGL);

	if (m_window == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// set opengl attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);

	// create opengl context
	m_context = SDL_GL_CreateContext(m_window);

	if (!gladLoadGL())
	{
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	return true;
}

void Renderer::Shutdown()
{
	// delete context
	SDL_GL_DeleteContext(m_context);

	// destroy window
	SDL_DestroyWindow(m_window);
}

void Renderer::Update()
{

}

void Renderer::ClearBuffer()
{
	glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
