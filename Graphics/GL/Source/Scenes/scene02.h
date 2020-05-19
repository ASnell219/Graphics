#pragma once
#include "core/scene.h"

class Scene02 : public Scene
{
public:
	Scene02(class Engine* engine) : Scene(engine) {}
	virtual ~Scene02() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene02"; }

private:
	Shader * m_shader = nullptr;
	VertexArray * m_vertexArray = nullptr;
	class Camera* m_camera = nullptr;
	
	glm::mat4 m_mxProjection;
	Transform m_transform;

	float m_direction = 1.0f;
};