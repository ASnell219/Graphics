#pragma once

#include "core/engine.h"

class Material
{
public:
	Material() {}

	static GLuint CreateTexture(GLuint width, GLuint height);
	void AddTexture(GLuint texture, GLuint activeTexture);
	bool AddTexture(const std::string& filename, GLuint activeTexture);
	bool AddTextureCube(const std::string& filename, const std::vector<std::string>& suffixes, 
						const std::string& extension, GLuint activeTexture);


	void SetMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);
	void SetTextures();

	static GLuint LoadTexture(const std::string& filename);
	static GLuint LoadTextureCube(const std::string& basename, const std::vector<std::string>& suffixes,
									const std::string& extension);
	static GLuint CreateDepthTexture(GLuint width, GLuint height);
	static GLuint CreateDepthbuffer(GLuint texture, GLuint width, GLuint height);

public:
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 100.0f;

	struct TextureInfo
	{
		GLenum type;
		GLuint activeTexture;
		GLuint texture;
	};

private:
	std::vector<TextureInfo> m_textures;
};