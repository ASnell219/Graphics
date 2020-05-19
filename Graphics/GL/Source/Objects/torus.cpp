#include "torus.h"
#include "core/scene.h"

void Torus::Initialize(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings)
{
	GLuint faces = nsides * nrings;
	int nVerts = nsides * (nrings + 1);   // One extra ring to duplicate first ring

	std::vector<GLfloat> positions(3 * nVerts);
	std::vector<GLfloat> normals(3 * nVerts);
	std::vector<GLfloat> uvs(2 * nVerts);
	std::vector<GLushort> indices(6 * faces);

	// Generate the vertex data
	float ringFactor = glm::two_pi<float>() / nrings;
	float sideFactor = glm::two_pi<float>() / nsides;
	int idx = 0, tidx = 0;
	for (GLuint ring = 0; ring <= nrings; ring++) {
		float u = ring * ringFactor;
		float cu = cos(u);
		float su = sin(u);
		for (GLuint side = 0; side < nsides; side++) {
			float v = side * sideFactor;
			float cv = cos(v);
			float sv = sin(v);
			float r = (outerRadius + innerRadius * cv);
			positions[idx] = r * cu;
			positions[idx + 1] = r * su;
			positions[idx + 2] = innerRadius * sv;
			normals[idx] = cv * cu * r;
			normals[idx + 1] = cv * su * r;
			normals[idx + 2] = sv * r;
			uvs[tidx] = u / glm::two_pi<float>();
			uvs[tidx + 1] = v / glm::two_pi<float>();
			tidx += 2;
			// Normalize
			float len = sqrt(normals[idx] * normals[idx] +
				normals[idx + 1] * normals[idx + 1] +
				normals[idx + 2] * normals[idx + 2]);
			normals[idx] /= len;
			normals[idx + 1] /= len;
			normals[idx + 2] /= len;
			idx += 3;
		}
	}

	idx = 0;
	for (GLuint ring = 0; ring < nrings; ring++) {
		GLuint ringStart = ring * nsides;
		GLuint nextRingStart = (ring + 1) * nsides;
		for (GLuint side = 0; side < nsides; side++) {
			int nextSide = (side + 1) % nsides;
			// The quad
			indices[idx] = (ringStart + side);
			indices[idx + 1] = (nextRingStart + side);
			indices[idx + 2] = (nextRingStart + nextSide);
			indices[idx + 3] = ringStart + side;
			indices[idx + 4] = nextRingStart + nextSide;
			indices[idx + 5] = (ringStart + nextSide);
			idx += 6;
		}
	}

	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, 3 * sizeof(GLfloat), nVerts, &positions[0]);
	m_vertexArrays.SetAttribute(0, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, 3 * sizeof(GLfloat), nVerts, &normals[0]);
	m_vertexArrays.SetAttribute(1, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, 2 * sizeof(GLfloat), nVerts, &uvs[0]);
	m_vertexArrays.SetAttribute(2, 2, (2 * sizeof(GLfloat)), 0);

	m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_SHORT, 6 * faces, &indices[0]);
}

