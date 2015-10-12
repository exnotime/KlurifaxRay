#pragma once
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/Model.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/VertexBuffer.h"
#define g_ModelBank ModelBank::GetInstance()
typedef int ModelHandle;
/*
This class loads and store all models and vertices and indices.
It also generates vertex and index buffers
*/
class ModelBank{
public:
	~ModelBank();
	static ModelBank& GetInstance();
	const Model& FetchModel(const char* filename);
	const Model& FetchModel(ModelHandle handle);
	ModelHandle LoadModel(const char* filename);
	ModelHandle AddModel(Model& TheModel);
	ModelHandle AddModel(vector<VertexPosNormalTexTangent>& vertices, vector<unsigned int>& indices, Model& outModel);
	void UpdateModel(ModelHandle& handle, Model& model);
	void BuildBuffers();
	void DeleteModel(ModelHandle& handle);
	void ApplyBuffers();
	void Clear();
	VertexBuffer& GetVertexBuffer();
	GLuint& GetIndexBuffer();
	vector<unsigned int> GetIndices();
	vector<VertexPosNormalTexTangent> GetVertices();
	vector<Model> GetModels();
private:
	//void LoadMaterials(Model& model, string filename, const aiScene* scene); //TODO: move to texturebank
	void LoadMeshes(Model& model, const aiScene* scene);
	ModelBank();
	VertexBuffer m_VertexBuffer;
	GLuint m_IndexBuffer;
	std::vector<VertexPosNormalTexTangent> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::map<ModelHandle, Model> m_Models;
	int m_Numerator = 0;
};