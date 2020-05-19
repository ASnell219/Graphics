#pragma once

#include "renderable.h"

class Light : public Renderable
{
public:
	Light(Scene* scene, const std::string& name = "") : Renderable(scene, name) {}
	
	virtual void Update();
	
public:
	glm::vec3 diffuse;
	glm::vec3 specular;
};