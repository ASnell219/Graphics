#pragma once
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <glad\glad.h>
#include "sdl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <map>

#include "system.h"
#include "timer.h"
#include "input.h"
#include "file_system.h"
#include "renderer/renderer.h"
#include "Math/transform.h"


class Engine
{
public:
	Engine();
	~Engine();

	bool Initialize();
	void Shutdown();
	void Update();
	
	// get engine system
	template <typename T>
	T* Get()
	{
		for (System* system : m_systems)
		{
			if (dynamic_cast<T*>(system) != nullptr)
			{
				return static_cast<T*>(system);
			}
		}
		return nullptr;
	}

	bool ForceQuit() { return m_quit; }

	SDL_Event& GetEvent() { return m_event; }

private:
	std::vector<class System*> m_systems;
	bool m_quit = false;
	SDL_Event m_event;
};