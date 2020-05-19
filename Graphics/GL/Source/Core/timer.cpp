#include "timer.h"
#include <algorithm>

bool Timer::Initialize()
{
	m_timeScale = 1.0f;
	m_paused = false;

	return true;
}

void Timer::Update()
{
	Uint32 ticks = SDL_GetTicks();
	Uint32 milliseconds = ticks - m_prevTicks;
	m_prevTicks = ticks;

	m_dt = (m_paused) ? 0.0f : milliseconds / 1000.0f;
	m_dt = std::min<float>(m_dt, 1.0f);
}

void Timer::Shutdown()
{
}

void Timer::Reset()
{
	m_prevTicks = SDL_GetTicks();
}
