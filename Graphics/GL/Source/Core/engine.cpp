#include "engine.h"

Engine::Engine()
{
	//
}

Engine::~Engine()
{
	//
}

bool Engine::Initialize()
{
	m_systems.push_back(new Timer(this));
	m_systems.push_back(new Input(this));
	m_systems.push_back(new FileSystem(this));
	m_systems.push_back(new Renderer(this));
	m_systems.push_back(new UI(this));

	for (System* system : m_systems)
	{
		if (system->Initialize() == false)
		{
			SDL_Log("Error initializing system (%s).\n", system->Name());
			return false;
		}
	}

	return true;
}

void Engine::Shutdown()
{
	for (System* system : m_systems)
	{
		system->Shutdown();
	}
}

void Engine::Update()
{

	SDL_PollEvent(&m_event);

	switch (m_event.type)
	{
	case SDL_QUIT:
		m_quit = true;
		break;
	case SDL_KEYDOWN:
		if (m_event.key.keysym.sym == SDLK_ESCAPE)
		{
			m_quit = true;
		}

	}

	SDL_PumpEvents();
	for (System* system : m_systems)
	{
		system->Update();
	}
}