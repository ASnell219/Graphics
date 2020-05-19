#pragma once
#include "core/engine.h"

class UI : public System
{
public:
	UI(Engine* engine);

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;

	void Draw();

	void Start();
	void End();

	const char* Name() { return "UI"; }

	static void Vec3Edit(const char* label, glm::vec3* vec3);
	static void Vec4Edit(const char* label, glm::vec4* vec4);
	static void QuatEdit(const char* label, glm::quat* quat);

	static bool VectorOfStringGetter(void* data, int n, const char** out_text);
};