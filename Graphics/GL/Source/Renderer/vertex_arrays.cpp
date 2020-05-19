#include "vertex_arrays.h"

VertexArrays::VertexArrays()
{ 
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}

VertexArrays::~VertexArrays()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	for (buffer_t buffer : m_buffers)
	{
		glDeleteBuffers(1, &buffer.id);
	}
	glDeleteBuffers(1, &m_indexBufferID);
}

void VertexArrays::Bind()
{
	assert(m_vertexArrayID != 0);
	glBindVertexArray(m_vertexArrayID);
}

void VertexArrays::CreateBuffer(eVertexType type, GLsizei vertexSize, GLsizei numVertex, void* data)
{
	GLuint vertexBufferID = 0;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * numVertex, data, GL_STATIC_DRAW);

	buffer_t buffer = { type, vertexBufferID, numVertex };
	m_buffers.push_back(buffer);
}

void VertexArrays::CreateIndexBuffer(GLenum indexType, GLsizei numIndex, void* data)
{
	m_indexType = indexType;
	m_numIndex = numIndex;

	glGenBuffers(1, &m_indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	size_t indexSize = (indexType == GL_UNSIGNED_SHORT) ? 2 : 4;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * numIndex, data, GL_STATIC_DRAW);
}

void VertexArrays::SetAttribute(eVertexType type, GLuint index, GLint size, GLsizei stride, void* offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(type).id);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
}


void VertexArrays::SetAttribute(GLuint index, GLint size, GLsizei stride, void* offset)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
}

void VertexArrays::Draw(GLenum primitiveType)
{
	glBindVertexArray(m_vertexArrayID);
	if (m_numIndex == 0)
	{
		glDrawArrays(primitiveType, 0, m_buffers[0].num);
	}
	else
	{
		glDrawElements(primitiveType, m_numIndex, m_indexType, 0);
	}
	glBindVertexArray(0);
}

VertexArrays::buffer_t VertexArrays::GetBuffer(eVertexType type)
{
	buffer_t result;

	for (buffer_t buffer : m_buffers)
	{
		if (buffer.type == type)
		{
			result = buffer;
			break;
		}
	}

	return result;
}