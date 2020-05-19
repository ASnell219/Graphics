#pragma once
#include "core/scene.h"

class Scene16 : public Scene
{
public:
	Scene16(class Engine* engine) : Scene(engine) {}
	virtual ~Scene16() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Shadows"; }

private:
	GLuint m_depthBuffer = 0;
	Shader* m_depthShader = nullptr;
	float m_time = 0.0f;
};