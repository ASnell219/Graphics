#include "plane.h"
#include "core/scene.h"

void Plane::Initialize(GLfloat xsize, GLfloat zsize, GLuint xSegments, GLuint zSegments, GLfloat sMax, GLfloat tMax)
{
	std::vector<GLfloat> positions(3 * (xSegments + 1) * (zSegments + 1));
	std::vector<GLfloat> normals(3 * (xSegments + 1) * (zSegments + 1));
	std::vector<GLfloat> uvs(2 * (xSegments + 1) * (zSegments + 1));
	std::vector<GLushort> indices(6 * xSegments * zSegments);

	float x2 = xsize / 2.0f;
	float z2 = zsize / 2.0f;
	float iFactor = (float)zsize / zSegments;
	float jFactor = (float)xsize / xSegments;
	float texi = sMax / zSegments;
	float texj = tMax / xSegments;
	float x, z;
	int vidx = 0, tidx = 0;
	for (GLuint i = 0; i <= zSegments; i++) {
		z = iFactor * i - z2;
		for (GLuint j = 0; j <= xSegments; j++) {
			x = jFactor * j - x2;
			positions[vidx] = x;
			positions[vidx + 1] = 0.0f;
			positions[vidx + 2] = z;
			normals[vidx] = 0.0f;
			normals[vidx + 1] = 1.0f;
			normals[vidx + 2] = 0.0f;

			uvs[tidx] = j * texi;
			uvs[tidx + 1] = i * texj;

			vidx += 3;
			tidx += 2;
		}
	}

	GLuint rowStart, nextRowStart;
	int idx = 0;
	for (GLuint i = 0; i < zSegments; i++) {
		rowStart = (GLuint)(i * (xSegments + 1));
		nextRowStart = (GLuint)((i + 1) * (xSegments + 1));
		for (GLuint j = 0; j < xSegments; j++) {
			indices[idx] = rowStart + j;
			indices[idx + 1] = nextRowStart + j;
			indices[idx + 2] = nextRowStart + j + 1;
			indices[idx + 3] = rowStart + j;
			indices[idx + 4] = nextRowStart + j + 1;
			indices[idx + 5] = rowStart + j + 1;
			idx += 6;
		}
	}

	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, 3 * sizeof(GLfloat), (GLsizei)positions.size() / 3, &positions[0]);
	m_vertexArrays.SetAttribute(0, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, 3 * sizeof(GLfloat), (GLsizei)normals.size() / 3, &normals[0]);
	m_vertexArrays.SetAttribute(1, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, 2 * sizeof(GLfloat), (GLsizei)uvs.size() / 2, &uvs[0]);
	m_vertexArrays.SetAttribute(2, 2, (2 * sizeof(GLfloat)), 0);

	m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), &indices[0]);
}

