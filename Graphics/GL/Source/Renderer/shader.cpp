#include "shader.h"

Shader::~Shader()
{
	// return if program was not set
	if (m_programID == 0)
	{
		return;
	}

	// get attached shaders and delete shaders
	GLint numShaders = 0;
	glGetProgramiv(m_programID, GL_ATTACHED_SHADERS, &numShaders);

	GLuint* shaders = new GLuint[numShaders];
	glGetAttachedShaders(m_programID, numShaders, NULL, shaders);
	for (int i = 0; i < numShaders; i++)
	{
		glDeleteShader(shaders[i]);
	}
	delete[] shaders;

	// delete program
	glDeleteProgram(m_programID);
}

void Shader::CompileShader(const std::string& filename, GLenum shaderType)
{
	// create program if not already created
	if (m_programID == 0)
	{
		m_programID = glCreateProgram();
	}

	// get shader string
	std::string shaderString;
	FileSystem::ReadFileToString(filename, shaderString);

	SDL_Log("Compiling Shader <%s>", filename.c_str());

	// create shader from string
	GLuint shaderID = CreateShader(shaderString, shaderType);
	if (shaderID != 0)
	{
		SDL_Log("Success.");
		glAttachShader(m_programID, shaderID);
	}
	
}

GLuint Shader::CreateShader(const std::string& source, GLenum shaderType)
{
	// create shader
	GLuint shaderID = glCreateShader(shaderType);

	// compile shader
	const char* sourse_c = source.c_str();
	glShaderSource(shaderID, 1, &sourse_c, NULL);
	glCompileShader(shaderID);

	// check shader compilation status
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
		SDL_Log("Error: Failed to compile shader.");
		SDL_Log("Shader Info: %s", &infoLog[0]);

		glDeleteShader(shaderID);
		shaderID = 0;
	}

	return shaderID;
}

void Shader::Link()
{
	assert(!m_isLinked && m_programID != 0);

	// link shader programs
	glLinkProgram(m_programID);

	// check program link status
	GLint status;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength > 0)
		{
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

			SDL_Log("Error: Failed to link program.");
			SDL_Log("Program Info: %s", &infoLog[0]);
		}
	
		glDeleteProgram(m_programID);

	}
	else
	{
		m_isLinked = true;
	}
}

void Shader::Use()
{
	glUseProgram(m_programID);

	for (SubroutineInfo info : m_subroutines)
	{
		glUniformSubroutinesuiv(info.shaderType, 1, &info.index);
	}
}

void Shader::AddSubroutine(GLenum shaderType, const std::string& subroutine)
{
	Use();

	GLuint subroutineIndex = glGetSubroutineIndex(m_programID, shaderType, subroutine.c_str());
	SubroutineInfo subroutineInfo = { shaderType, subroutineIndex };

	m_subroutines.push_back(subroutineInfo);
}

void Shader::SetUniform(const std::string& name, float x, float y, float z)
{
	GLint uniform = GetUniform(name);
	glUniform3f(uniform, x, y, z);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& v2)
{
	GLint uniform = GetUniform(name);
	glUniform2f(uniform, v2.x, v2.y);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& v3)
{
	GLint uniform = GetUniform(name);
	glUniform3f(uniform, v3.x, v3.y, v3.z);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& v4)
{
	GLint uniform = GetUniform(name);
	glUniform4f(uniform, v4.x, v4.y, v4.z, v4.w);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& mx4)
{
	GLint uniform = GetUniform(name);
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mx4[0][0]);
}

void Shader::SetUniform(const std::string& name, const glm::mat3& mx3)
{
	GLint uniform = GetUniform(name);
	glUniformMatrix3fv(uniform, 1, GL_FALSE, &mx3[0][0]);
}

void Shader::SetUniform(const std::string& name, float vfloat)
{
	GLint uniform = GetUniform(name);
	glUniform1f(uniform, vfloat);
}

void Shader::SetUniform(const std::string& name, int vint)
{
	GLint uniform = GetUniform(name);
	glUniform1i(uniform, vint);
}

void Shader::SetUniform(const std::string& name, bool vbool)
{
	GLint uniform = GetUniform(name);
	glUniform1i(uniform, vbool);
}

void Shader::SetUniform(const std::string& name, GLuint vuint)
{
	GLint uniform = GetUniform(name);
	glUniform1ui(uniform, vuint);
}

GLint Shader::GetUniform(const std::string& name)
{
	auto uniform = m_uniforms.find(name);
	if (uniform == m_uniforms.end())
	{
		m_uniforms[name] = glGetUniformLocation(m_programID, name.c_str());
		if (m_uniforms[name] == -1)
		{
			SDL_Log("Error: %s uniform does not exist in the shader.\n", name.c_str());
		}
	}

	return m_uniforms[name];
}

