#pragma once
#include "core/engine.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_arrays.h"
#include "material.h"
#include "ui.h"

class Renderer : public System
{
public:
	Renderer(Engine* engine) : System(engine) {}
	virtual ~Renderer() {}

	bool Initialize();
	void Shutdown();
	void Update();

	const char* Name() { return "Renderer"; }

	void ClearBuffer();
	void SwapBuffer() { SDL_GL_SwapWindow(m_window); }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	friend class UI;

private:
	SDL_Window * m_window = nullptr;
	SDL_GLContext m_context;
	int m_width;
	int m_height;
};