#pragma once
#include "core/scene.h"

class Scene08 : public Scene
{
public:
	Scene08(class Engine* engine) : Scene(engine) {}
	virtual ~Scene08() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene08"; }

private:
	int m_lightIndex = 0;
};