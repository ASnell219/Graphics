#pragma once
#include "core/engine.h"

class VertexArrays
{
public:
	enum class eVertexType
	{
		POSITION,
		NORMAL,
		COLOR,
		TEXCOORD,
		TANGENT,
		MULTI
	};

	struct buffer_t
	{
		eVertexType type;
		GLuint id;
		GLsizei num;
	};

public:
	VertexArrays();
	virtual ~VertexArrays();

	void Bind();

	void CreateBuffer(eVertexType type, GLsizei vertexSize, GLsizei numVertex, void* data);
	void CreateIndexBuffer(GLenum indexType, GLsizei numIndex, void* data);
	
	void SetAttribute(eVertexType type, GLuint index, GLint size, GLsizei stride, void* offset);
	void SetAttribute(GLuint index, GLint size, GLsizei stride, void* offset);

	void Draw(GLenum primitiveType = GL_TRIANGLES);

private:
	buffer_t GetBuffer(eVertexType type);

private:
	GLuint m_vertexArrayID = 0;
	GLuint m_indexBufferID = 0;
	GLsizei m_numIndex = 0;
	GLenum m_indexType = GL_UNSIGNED_SHORT;

	std::vector<buffer_t> m_buffers;
};