#pragma once
#include "system.h"
#include "engine.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

class  Input : public System
{
public:
	enum eButtonState
	{
		IDLE,
		PRESSED,
		HELD,
		RELEASED
	};

	enum eDevice
	{
		KEYBOARD,
		MOUSE,
		CONTROLLER
	};

	enum eAxis
	{
		X,
		Y
	};

	struct InputInfo
	{
		int id;
		eDevice device;
		int index;
	};

	struct ControllerInfo
	{
		SDL_GameController* controller;

		Uint8 buttonstate[SDL_CONTROLLER_BUTTON_MAX];
		Uint8 prevButtonstate[SDL_CONTROLLER_BUTTON_MAX];
		float axis[SDL_CONTROLLER_AXIS_MAX];
		float prevAxis[SDL_CONTROLLER_AXIS_MAX];
	};

public:
	Input(Engine* engine) : System(engine) {}
	virtual ~Input() {}
	
	bool Initialize();
	void Shutdown();
	void Update();

	const char* Name() override { return "Input Manager"; }

	void AddAction(const std::string& action, int id, eDevice device, int index = 0);
	eButtonState GetActionButton(const std::string& action);
	float GetActionAxisAbsolute(const std::string& action);
	float GetActionAxisRelative(const std::string& action);

	eButtonState GetButtonState(int id, eDevice device = eDevice::KEYBOARD, int index = 0);
	float GetAxisAbsolute(int id, eDevice device = eDevice::MOUSE, size_t index = 0);
	float GetAxisRelative(int id, eDevice device = eDevice::MOUSE, size_t index = 0);

protected:
	bool GetButtonDown(int id, eDevice device, int index = 0);
	bool GetPreviousButtonDown(int id, eDevice device, int index = 0);

private:
	// keyboard
	Uint8* m_keystate;
	Uint8* m_prevKeystate;
	int m_numKeys;

	// mouse
	Uint32 m_mouseButtonstate;
	Uint32 m_prevMouseButtonstate;
	glm::vec2 m_mousePosition;
	glm::vec2 m_prevMousePosition;
		
	// controller
	std::vector<ControllerInfo> m_controllers;

	std::map<std::string, InputInfo> m_actions;
};
