#pragma once
#include "core/engine.h"

class Shader
{
public:
	Shader() {}
	~Shader();

public:
	void CompileShader(const std::string& filename, GLenum shaderType);
	GLuint CreateShader(const std::string& source, GLenum shaderType);

	void Link();
	void Use();

	GLuint GetProgramID() { return m_programID; }
	bool IsLinked() { return m_isLinked; }

	void AddSubroutine(GLenum shaderType, const std::string& subroutine);

	void SetUniform(const std::string& name, float x, float y, float z);
	void SetUniform(const std::string& name, const glm::vec2& v2);
	void SetUniform(const std::string& name, const glm::vec3& v3);
	void SetUniform(const std::string& name, const glm::vec4& v4);
	void SetUniform(const std::string& name, const glm::mat4& mx4);
	void SetUniform(const std::string& name, const glm::mat3& mx3);
	void SetUniform(const std::string& name, float vfloat);
	void SetUniform(const std::string& name, int vint);
	void SetUniform(const std::string& name, bool vbool);
	void SetUniform(const std::string& name, GLuint vuint);

private:
	GLint GetUniform(const std::string& name);
		
private:
	struct SubroutineInfo
	{
		GLenum shaderType;
		GLuint index;
	};

	GLuint m_programID = 0;
	bool m_isLinked = false;
	std::map<std::string, GLuint> m_uniforms;
	std::vector<SubroutineInfo> m_subroutines;
};
