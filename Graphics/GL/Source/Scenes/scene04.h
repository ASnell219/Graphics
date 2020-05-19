#pragma once
#include "core/scene.h"

class Scene04 : public Scene
{
public:
	Scene04(class Engine* engine) : Scene(engine) {}
	virtual ~Scene04() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene04"; }

	
};