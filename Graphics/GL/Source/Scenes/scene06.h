#pragma once
#include "core/scene.h"

class Scene06 : public Scene
{
public:
	Scene06(class Engine* engine) : Scene(engine) {}
	virtual ~Scene06() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene06"; }

private:
	int m_lightIndex = 0;
};