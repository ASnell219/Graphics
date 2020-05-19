#include "material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Material::SetMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

GLuint Material::CreateTexture(GLuint width, GLuint height)
{
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;

}

void Material::AddTexture(GLuint textureID, GLuint activeTexture)
{
	TextureInfo textureInfo = { GL_TEXTURE_2D, activeTexture, textureID };
	m_textures.push_back(textureInfo);
}

bool Material::AddTexture(const std::string& filename, GLuint activeTexture)
{
	GLuint textureID = LoadTexture(filename);
	if (textureID != 0)
	{
		TextureInfo textureInfo = { GL_TEXTURE_2D, activeTexture, textureID };
		m_textures.push_back(textureInfo);
	}

	return (textureID != 0);
}

bool Material::AddTextureCube(const std::string& filename, const std::vector<std::string>& suffixes, const std::string& extension, GLuint activeTexture)
{
	glActiveTexture(activeTexture);

	GLuint textureID = LoadTextureCube(filename, suffixes, extension);
	if (textureID != 0)
	{
		TextureInfo textureInfo = { GL_TEXTURE_CUBE_MAP, activeTexture, textureID };
		m_textures.push_back(textureInfo);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return (textureID != 0);
}

void Material::SetTextures()
{
	for (TextureInfo textureInfo : m_textures)
	{
		glActiveTexture(textureInfo.activeTexture);
		glBindTexture(textureInfo.type, textureInfo.texture);
	}
}

GLuint Material::LoadTexture(const std::string& filename)
{
	GLuint textureID = 0;

	int width;
	int height;
	int n;

	SDL_Log("Loading Texture <%s>", filename.c_str());

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);

	if (image == nullptr)
	{
		SDL_Log("Error: Could not load texture <%s>", filename.c_str());
	}
	
	if (image)
	{
		SDL_Log("Success");

		GLenum storageFormat = (n == 4) ? GL_RGBA8 : GL_RGB8;
		GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;
				
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexStorage2D(GL_TEXTURE_2D, 1, storageFormat, width, height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, imageFormat, GL_UNSIGNED_BYTE, image);

		stbi_image_free(image);
	}

	return textureID;
}

GLuint Material::LoadTextureCube(const std::string& basename, const std::vector<std::string>& suffixes, const std::string& extension)
{
	GLuint targets[] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	GLuint textureID = 0;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (int i = 0; i < 6; i++)
	{
		int width;
		int height;
		int n;

		std::string filename = basename + suffixes[i] + "." + extension;
		SDL_Log("Loading Texture <%s>", filename.c_str());

		unsigned char* image = stbi_load(filename.c_str(), &width, &height, &n, 0);
		assert(image);

		GLenum storageFormat = (n == 4) ? GL_RGBA8 : GL_RGB8;
		GLenum imageFormat = (n == 4) ? GL_RGBA : GL_RGB;

		glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, storageFormat, width, height);
		glTexSubImage2D(targets[i], 0, 0, 0, width, height, imageFormat, GL_UNSIGNED_BYTE, image);

		stbi_image_free(image);
	}

	return textureID;
}

GLuint Material::CreateDepthTexture(GLuint width, GLuint height)
{
	GLuint depthTexture;

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	return depthTexture;
}

GLuint Material::CreateDepthbuffer(GLuint texture, GLuint width, GLuint height)
{
	GLuint fboHandle;

	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	glDrawBuffer(GL_NONE);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fboHandle;
}

