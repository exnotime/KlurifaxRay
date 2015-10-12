#pragma once
#include "soil/SOIL.h"
#include "GL/glew.h"
#include <stdio.h>
#include <string>
typedef int TextureHandle;
enum TextureType
{
	TEXTURE_2D,
	CUBE_TEXTURE
};
class Texture
{
private:
	GLuint m_Handle;
	int m_Width,m_Height;
	TextureType m_Type;
	bool m_Loaded;
	std::string m_Filename;
	GLuint64 m_GPUAddress;
public:
	Texture();
	~Texture();
	bool Init(const char* Filename,TextureType Type);
	bool Init(char* data, int channels,int width,int height, TextureType Type);
	bool InitLinear(const char* filename, int width, int height);
	GLuint GetHandle();
	void Apply(GLuint location,int index);
	TextureType GetType();
	bool GetLoaded();
	void SetFilename(const char* filename);
	std::string GetFilename();
	GLuint64 GetAddress();
};