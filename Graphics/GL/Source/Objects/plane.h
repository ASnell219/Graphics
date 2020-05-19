#pragma once

#include "model.h"

class Plane : public Model
{
public:
	Plane(Scene* scene, const std::string& name = "") : Model(scene, name) {}

	void Initialize(GLfloat xsize, GLfloat zsize, GLuint xSegments, GLuint zSegments, GLfloat sMax, GLfloat tMax);
};