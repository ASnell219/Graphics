#include "model.h"
#include "camera.h"
#include "core/scene.h"

void Model::Update()
{
	transform.Update();

	// update shader
	m_shader.Use();

	Camera* camera = scene->GetObject<Camera>(m_cameraName);

	// update normals
	glm::mat3 mxNormal = glm::mat3(camera->transform.matrix * transform.matrix);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader.SetUniform("normal_matrix", mxNormal);

	// update model view
	glm::mat4 mxMV = camera->transform.matrix * transform.matrix;
	m_shader.SetUniform("model_view_matrix", mxMV);

	// update mvp
	glm::mat4 mxMVP = camera->projection * camera->transform.matrix * transform.matrix;
	m_shader.SetUniform("model_view_projection_matrix", mxMVP);
}

void Model::Draw()
{
	m_shader.Use();
	m_material.SetTextures();
	m_vertexArrays.Draw();
}

//void Model::Edit()
//{
	//Material::Edit(&m_material);
	//Transform::Edit(&transform);
//}

bool Model::Import(const std::string& filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	assert(scene);

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textcoords;
	std::vector < glm::vec3> tangents;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		positions.push_back(position);

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		normals.push_back(normal);

		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		tangents.push_back(tangent);

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textcoord;
			textcoord.x = mesh->mTextureCoords[0][i].x;
			textcoord.y = mesh->mTextureCoords[0][i].y;
			textcoords.push_back(textcoord);
		}
	}

	m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::POSITION, sizeof(glm::vec3), (GLsizei)positions.size(), &positions[0]);
	m_vertexArrays.SetAttribute(0, 3, sizeof(glm::vec3), 0);
	if (normals.size() > 0)
	{
		m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::NORMAL, sizeof(glm::vec3), (GLsizei)normals.size(), &normals[0]);
		m_vertexArrays.SetAttribute(1, 3, sizeof(glm::vec3), 0);
	}
	if (textcoords.size() > 0)
	{
		m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TEXCOORD, sizeof(glm::vec2), (GLsizei)textcoords.size(), &textcoords[0]);
		m_vertexArrays.SetAttribute(2, 2, sizeof(glm::vec2), 0);
	}
	if (tangents.size() > 0)
	{
		m_vertexArrays.CreateBuffer(VertexArrays::eVertexType::TANGENT, sizeof(glm::vec3), (GLsizei)tangents.size(), &tangents[0]);
		m_vertexArrays.SetAttribute(3, 3, sizeof(glm::vec3), 0);
	}
	
	std::vector<GLuint> indices;
	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	m_vertexArrays.CreateIndexBuffer(GL_UNSIGNED_INT, (GLsizei)indices.size(), &indices[0]);
}
