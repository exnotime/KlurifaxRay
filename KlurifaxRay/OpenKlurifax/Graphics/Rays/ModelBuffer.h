#pragma once
#include <vector>
#include <glm/glm.hpp>
#include"../Model.h"
#include "../Vertex.h"
using glm::vec4;
using glm::ivec4;
using std::vector;

struct Vertex{
	vec4 Position;
	vec4 Normal;
	vec4 TexCoord;
	vec4 Tangent;
};
/*
struct Mesh{
uint VertexBufferOffset;
uint IndexBufferOffset;
uint Indices;
uint Material;
};
*/
struct ShaderModel{
	ivec4 IndexVertexSizeISizeV;
	ivec4 MaterialSizeM;
	/*
	uint IndexHandle; //place in ModelIndexBuffer
	uint VertexHandle; //place in ModelVertexBuffer
	uint NumVertices;
	uint NumIndices;
	uint MaterialBasePointer;
	uint NrOfMeshes;
	*/
	vec4 ModelMinBounds;
	vec4 ModelMaxBounds;
	ivec4 Meshes[20]; //Mesh struct broken down to 4 ints
	ivec4 MeshTextures[20];
	vec4 MeshMinBounds[20];
	vec4 MeshMaxBounds[20];

};

#define MaxNrOfModels 50
#define MaxNrIndices 100000
#define MaxNrVertices 50000
#define ModelBufferSize sizeof(unsigned int) * MaxNrIndices + sizeof(Vertex) * MaxNrVertices + sizeof(ShaderModel) * MaxNrOfModels

#define ModelBufferPos 0
#define IndicesBufferPos ModelBufferPos + sizeof(ShaderModel) * MaxNrOfModels
#define VerticesBuffferPos IndicesBufferPos + sizeof(unsigned int) * MaxNrIndices

class ModelBuffer{
public:
	ModelBuffer();
	~ModelBuffer();
	void Init();
	void Build();
	void Bind(GLuint program, GLuint blockBinding);

	GLuint GetBuffer();
	void AddModels(vector<Model>& models, vector<VertexPosNormalTexTangent>& vertices, vector<unsigned int>& indices);
private:
	vector<ShaderModel>			m_Models;
	vector<Vertex>			m_Vertices;
	vector<unsigned int>	m_Indices;
	GLuint					m_Buffer;
};