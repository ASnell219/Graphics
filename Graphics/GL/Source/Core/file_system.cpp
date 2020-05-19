#include "file_system.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool FileSystem::Initialize()
{
	return true;
}

void FileSystem::Shutdown()
{
	//
}

void FileSystem::Update()
{
	//
}

bool FileSystem::ReadFileToString(const std::string& filename, std::string& filestring)
{
	std::ifstream fileStream(filename, std::ios::in);
	if (!fileStream.is_open())
	{
		SDL_Log("Error: Failed to open file: %s", filename);
		return false;
	}

	std::stringstream content;
	content << fileStream.rdbuf();
	fileStream.close();

	filestring = content.str();

	return true;
}
