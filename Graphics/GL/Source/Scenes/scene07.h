#pragma once
#include "core/scene.h"

class Scene07 : public Scene
{
public:
	Scene07(class Engine* engine) : Scene(engine) {}
	virtual ~Scene07() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene07"; }

private:
	int m_lightIndex = 0;
};