#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void)
{
}

VertexBuffer::VertexBuffer(VertexType VertexType,void* Data,unsigned int Size,GLuint BindingIndex)
{
	Init(VertexType,Data,Size,BindingIndex);
}

VertexBuffer::~VertexBuffer(void)
{
	glDeleteBuffers(1,&m_Handle);
}

bool VertexBuffer::Init(VertexType VertexType,void* Data,unsigned int Size,GLuint BindingIndex)
{
	glGenVertexArrays(1,&m_VertexArrayId);
	glBindVertexArray(m_VertexArrayId);
	glGenBuffers(1,&m_Handle);
	if(m_Handle == GL_INVALID_VALUE)
		return false;
	glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
	glBufferData(GL_ARRAY_BUFFER,Size,Data,GL_STATIC_DRAW);

	if(VertexType == POS)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexPos),nullptr);
	}
	else if(VertexType == POS_NORMAL)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexPosNormal),nullptr);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexPosNormal),(unsigned char*)nullptr + (3 * sizeof(float)));

	}
		else if(VertexType == POS_TEX)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexPosTex),nullptr);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(VertexPosTex),(unsigned char*)nullptr + (3 * sizeof(float)));

	}
		else if (VertexType == POS_NORMAL_TEX)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexPosNormalTex),0);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexPosNormalTex),(unsigned char*)NULL + (3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexPosNormalTex),(unsigned char*)NULL + (6 * sizeof(float)));
	}
	else if (VertexType == POS_NORMAL_TEX_TANGENT)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTexTangent), 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTexTangent), (unsigned char*)NULL + (3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTexTangent), (unsigned char*)NULL + (6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosNormalTexTangent), (unsigned char*)NULL + (8 * sizeof(float)));
	}
	return glIsBuffer(m_Handle) == GL_TRUE;
}

bool VertexBuffer::Init(InputLayout Layout, void* Data,unsigned int Size,GLuint BindingIndex)
{
	glGenVertexArrays(1,&m_VertexArrayId);
	glBindVertexArray(m_VertexArrayId);
	glGenBuffers(1,&m_Handle);
	if(m_Handle == GL_INVALID_VALUE)
		return false;
	glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
	glBufferData(GL_ARRAY_BUFFER,Size,Data,GL_STATIC_DRAW);
	int currentOffset = 0; 
	for(int i = 0; i < Layout.Attributes.size();++i)
	{
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER,m_Handle);
		glVertexAttribPointer(i,Layout.Attributes[i].Count,Layout.Attributes[i].Type,GL_FALSE,Layout.Attributes[i].Size,(unsigned char*)NULL +(Layout.Attributes[i].Offset));
	}
	return glIsBuffer(m_Handle) == GL_TRUE;
}

void VertexBuffer::Apply()
{
	glBindVertexArray(m_VertexArrayId);
}

void* VertexBuffer::Map(GLenum Access)
{
	return glMapBuffer(GL_ARRAY_BUFFER,Access);
}

void VertexBuffer::UnMap()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VertexBuffer::Release(){
	glDeleteBuffers(1, &m_Handle);
}

