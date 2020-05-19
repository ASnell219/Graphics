#pragma once

#include "model.h"

class Torus : public Model
{
public:
	Torus(Scene* scene, const std::string& name = "") : Model(scene, name) {}

	void Initialize(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings);
};