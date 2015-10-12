#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"
using std::vector;
enum VertexType
{
	POS,
	POS_NORMAL,
	POS_TEX,
	POS_NORMAL_TEX,
	POS_NORMAL_TEX_TANGENT
};
struct InputLayout
{
	struct Attribute
	{
		GLenum Type;
		int Count;
		int Size;
		int Offset;
	};
	vector<Attribute> Attributes;
};
class VertexBuffer
{
private:
	GLuint m_Handle;
	GLuint m_VertexArrayId;
public:
	VertexBuffer(void);
	VertexBuffer(VertexType VertexType,void* Data,unsigned int Size,GLuint BindingIndex);
	~VertexBuffer(void);
	bool Init(VertexType VertexType,void* Data,unsigned int Size,GLuint BindingIndex);
	bool Init(InputLayout Layout, void* Data,unsigned int Size,GLuint BindingIndex);
	void Apply();
	void* Map(GLenum Access);
	void UnMap();
	void Release();
};

