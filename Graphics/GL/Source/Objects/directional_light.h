#pragma once

#include "light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(Scene* scene, const std::string& name = "");

	void Update() override;
	void Draw() override;

	void SetDirection(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 GetDirectionFromView(const glm::mat4& mxView);

protected:
	class Cube* m_model = nullptr;
};