#pragma once
#include "core/engine.h"

struct Transform
{
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 matrix = glm::mat4(1.0f);

	void Update()
	{
		glm::mat4 mxt	= glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mxr	= glm::mat4_cast(rotation);
		glm::mat4 mxs	= glm::scale(glm::mat4(1.0f), scale);

		matrix = mxt * mxr * mxs;
	}

	static void Edit(Transform* const transform)
	{
		ImGui::DragFloat3("Translation", (float*)&transform->translation, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat4("Rotation", (float*)&transform->rotation, 0.1f, -100.0f, 100.0f);
		ImGui::DragFloat3("Scale", (float*)&transform->scale, 0.1f, 0.0f, 10.0f);
	}
};