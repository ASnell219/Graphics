#include "sphere.h"
#include "core/scene.h"

void Sphere::Initialize(GLfloat radius, GLuint slices, GLuint stacks)
{
	int nVerts = (slices + 1) * (stacks + 1);
	int elements = (slices * 2 * (stacks - 1)) * 3;

	std::vector<GLfloat> positions(3 * nVerts);
	std::vector<GLfloat> normals(3 * nVerts);
	std::vector<GLfloat> uvs(2 * nVerts);
	std::vector<GLushort> indices(elements);

	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			positions[idx] = radius * nx; positions[idx + 1] = radius * ny; positions[idx + 2] = radius * nz;
			normals[idx] = nx; normals[idx + 1] = ny; normals[idx + 2] = nz;
			idx += 3;

			uvs[tIdx] = s;
			uvs[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				indices[idx] = stackStart;
				indices[idx + 1] = stackStart + 1;
				indices[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				indices[idx] = stackStart + j;
				indices[idx + 1] = stackStart + j + 1;
				indices[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				indices[idx] = stackStart + j;
				indices[idx + 1] = stackStart + j + 1;
				indices[idx + 2] = nextStackStart + j + 1;
				indices[idx + 3] = nextStackStart + j;
				indices[idx + 4] = stackStart + j;
				indices[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}

	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, 3 * sizeof(GLfloat), nVerts, &positions[0]);
	m_vertexArrays.SetAttribute(0, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, 3 * sizeof(GLfloat), nVerts, &normals[0]);
	m_vertexArrays.SetAttribute(1, 3, (3 * sizeof(GLfloat)), 0);
	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, 2 * sizeof(GLfloat), nVerts, &uvs[0]);
	m_vertexArrays.SetAttribute(2, 2, (2 * sizeof(GLfloat)), 0);
	
	m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_SHORT, elements, &indices[0]);
}


