#pragma once

#include "math/transform.h"

class Object
{
public:
	Object(class Scene* scene, const std::string& name = "") : scene(scene), name(name) {}
	virtual ~Object() {}

	virtual void Update() = 0;

public:
	std::string name;
	class Scene* scene;
	Transform transform;
};
