#pragma once

#include "model.h"

class Cube : public Model
{
public:
	Cube(Scene* scene, const std::string& name = "") : Model(scene, name) {}

	void Initialize(GLfloat size);
};