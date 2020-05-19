#pragma once

class System
{
public:
	System(class Engine* engine) : m_engine(engine) {}
	virtual ~System() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;

	virtual const char* Name() = 0;

protected:
	class Engine* m_engine;
};