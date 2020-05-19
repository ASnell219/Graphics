#include "vertex_array.h"

VertexArray::VertexArray(Engine* engine)
{ 
	m_engine = engine;
		
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteBuffers(1, &m_indexBufferID);
}

void VertexArray::Bind()
{
	assert(m_vertexArrayID != 0);
}

void VertexArray::CreateBuffer(GLsizei vertexSize, GLsizei numVertex, void* data)
{
	m_numVertex = numVertex;

	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertex, data, GL_STATIC_DRAW);
}

void VertexArray::CreateIndexBuffer(GLenum indexType, GLsizei numIndex, void* data)
{
	m_indexType = indexType;
	m_numIndex = numIndex;

	glGenBuffers(1, &m_indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	size_t indexSize = (indexType == GL_UNSIGNED_SHORT) ? 2 : 4;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * numIndex, data, GL_STATIC_DRAW);

}

void VertexArray::SetAttribute(GLuint index, GLint size, GLsizei stride, void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
}

void VertexArray::Draw(GLenum primitiveType)
{
	if (m_numIndex == 0)
	{
		glDrawArrays(primitiveType, m_numIndex, m_numVertex);
	}
	else
	{
		glDrawElements(primitiveType, m_numIndex, m_indexType, 0);
	}
}
