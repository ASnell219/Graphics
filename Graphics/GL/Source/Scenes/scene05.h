#pragma once
#include "core/scene.h"

class Scene05 : public Scene
{
public:
	Scene05(class Engine* engine) : Scene(engine) {}
	virtual ~Scene05() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene05"; }
};