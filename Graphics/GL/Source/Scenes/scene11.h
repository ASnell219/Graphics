#pragma once
#include "core/scene.h"

class Scene11 : public Scene
{
public:
	Scene11(class Engine* engine) : Scene(engine) {}
	virtual ~Scene11() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene11"; }

private:
	float m_time = 0.0f;
};