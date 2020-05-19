#pragma once
#include "engine.h"
#include "system.h"

class FileSystem : public System
{
public:
	FileSystem(Engine* engine) : System(engine) {}
	virtual ~FileSystem() {}

	bool Initialize();
	void Shutdown();
	void Update();

	const char* Name() { return "FileSystem"; }

	static bool ReadFileToString(const std::string& filename, std::string& filestring);

	void SetPathname(const std::string& pathname) { m_pathname = pathname; }
	const std::string& GetPathname() { return m_pathname; }
	
protected:
	std::string m_pathname;
};