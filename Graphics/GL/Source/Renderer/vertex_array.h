#pragma once
#include "core/engine.h"

class VertexArray
{
public:
	VertexArray(Engine* engine);
	virtual ~VertexArray();

	void Bind();

	void CreateBuffer(GLsizei vertexSize, GLsizei numVertex, void* data);
	void CreateIndexBuffer(GLenum indexType, GLsizei numIndex, void* data);
	
	void SetAttribute(GLuint index, GLint size, GLsizei stride, void* offset);

	void Draw(GLenum primitiveType = GL_TRIANGLES);

private:
	Engine * m_engine = nullptr;

	GLuint m_vertexArrayID = 0;
	GLuint m_vertexBufferID = 0;
	GLuint m_indexBufferID = 0;

	GLsizei m_numVertex = 0;
	GLsizei m_numIndex = 0;
	GLenum m_indexType = GL_UNSIGNED_SHORT;
};