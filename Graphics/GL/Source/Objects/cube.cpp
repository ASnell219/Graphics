#include "cube.h"
#include "core/scene.h"

void Cube::Initialize(GLfloat size)
{
	GLfloat side2 = size / 2.0f;

	std::vector<GLfloat> positions = {
		// Front
	   -side2, -side2, side2, side2, -side2, side2, side2,  side2, side2,  -side2,  side2, side2,
	   // Right
		side2, -side2, side2, side2, -side2, -side2, side2,  side2, -side2, side2,  side2, side2,
		// Back
		-side2, -side2, -side2, -side2,  side2, -side2, side2,  side2, -side2, side2, -side2, -side2,
		// Left
		-side2, -side2, side2, -side2,  side2, side2, -side2,  side2, -side2, -side2, -side2, -side2,
		// Bottom
		-side2, -side2, side2, -side2, -side2, -side2, side2, -side2, -side2, side2, -side2, side2,
		// Top
		-side2,  side2, side2, side2,  side2, side2, side2,  side2, -side2, -side2,  side2, -side2
	};

	std::vector<GLfloat> normals = {
		// Front
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	std::vector<GLfloat> uvs = {
		// Front
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// Right
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// Back
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// Left
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// Bottom
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// Top
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	std::vector<GLushort> indices = {
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
	};

	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, 3 * sizeof(GLfloat), (GLsizei)positions.size() / 3, &positions[0]);
	m_vertexArrays.SetAttribute(0, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, 3 * sizeof(GLfloat), (GLsizei)normals.size() / 3, &normals[0]);
	m_vertexArrays.SetAttribute(1, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, 2 * sizeof(GLfloat), (GLsizei)uvs.size() / 2, &uvs[0]);
	m_vertexArrays.SetAttribute(2, 2, (2 * sizeof(GLfloat)), 0);

	m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), &indices[0]);
}

