#pragma once
#include "Texture.h"
#include "Geometry.h"
#include "Lights.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

struct Mesh{
	unsigned int VertexBufferOffset;
	unsigned int IndexBufferOffset;
	int Indices;
	int Size;
	int Material;
	Box BoundingVolume;
};

struct Model{
public:
	unsigned int IndexHandle; //place in ModelIndexBuffer
	unsigned int VertexHandle; //place in ModelVertexBuffer
	string Name;
	Box BoundingBox;
	int NumVertices;
	int NumIndices;
	int MaterialBasePointer;
	int TextureCount;
	bool Loaded;
	vector<Mesh> Meshes;
};