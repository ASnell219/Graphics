#pragma once
#include "engine.h"
#include "object.h"

class Scene
{
public:
	Scene(class Engine* engine) : m_engine(engine) {}
	virtual ~Scene() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual const char* Name() = 0;

public:
	void AddObject(Object* object)
	{
		m_objects.push_back(object);
	}

	template<typename T>
	T* CreateObject(const std::string& name = "")
	{
		T* object = new T(this, name);
		AddObject(object);

		return object;
	}

	template<typename T>
	T* GetObject(const std::string& name = "")
	{
		for (auto object : m_objects)
		{
			if (name.empty() || object->name == name)
			{
				T* result = dynamic_cast<T*>(object);
				if (result)
				{
					return result;
				}
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> objects;
		for (auto object : m_objects)
		{
			if (dynamic_cast<T*>(object) != nullptr)
			{
				objects.push_back(dynamic_cast<T*>(object));
			}
		}

		return objects;
	}

public:
	class Engine* m_engine;

protected:
	glm::vec2 m_uvOffset = glm::vec2(0.0f, 0.0f);
	std::vector<Object*> m_objects;
	
};