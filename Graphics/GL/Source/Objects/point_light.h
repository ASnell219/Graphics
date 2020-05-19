#pragma once

#include "light.h"

class PointLight : public Light
{
public:
	PointLight(Scene* scene, const std::string& name = "");

	void Update() override;
	void Draw() override;

	glm::vec4 GetPositionFromView(const glm::mat4& mxView);

protected:
	class Cube* m_model = nullptr;
};