#pragma once
#include "core/scene.h"

class Scene12 : public Scene
{
public:
	Scene12(class Engine* engine) : Scene(engine) {}
	virtual ~Scene12() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene12"; }

private:
	float m_time = 0.0f;
	GLuint m_depthBuffer = 0; 
	Shader* m_depthShader = nullptr;
};