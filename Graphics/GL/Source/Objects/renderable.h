#pragma once

#include "core/object.h"
#include "core/engine.h"

class Renderable : public Object
{
public:
	Renderable(Scene* scene, const std::string& name = "") : Object(scene, name) {}
	virtual ~Renderable() {}

	virtual void Draw() = 0;
};