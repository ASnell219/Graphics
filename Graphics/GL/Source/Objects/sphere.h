#pragma once

#include "objects/model.h"

class Sphere : public Model
{
public:
	Sphere(Scene* scene, const std::string& name = "") : Model(scene, name) {}

	void Initialize(GLfloat radius, GLuint slices, GLuint stacks);
};