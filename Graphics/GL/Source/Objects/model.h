#pragma once

#include "renderable.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

class Model : public Renderable
{
public:
	Model(Scene* scene, const std::string& name = "") : Renderable(scene, name) {}

	virtual void Update();
	void Draw();

	//void Edit();
	bool Import(const std::string& filename);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

public:
	VertexArrays m_vertexArrays;
	Shader m_shader;
	Material m_material;
	std::string m_cameraName;
};
