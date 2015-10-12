#pragma once 
#include <vector>
#include <map>
#include "GL/glew.h"
#include "../Texture.h"
#define MaxNrOfTextures 200
#define BufferSize MaxNrOfTexture * sizeof(GLuint64)

using std::vector;
using std::map;
class TextureBuffer{
public:
	TextureBuffer();
	~TextureBuffer();

	void Init();
	void Bind(GLuint program, GLuint bindingPoint);
	void Build();
	void AddTextures(map<TextureHandle, Texture>& textures);
private:
	GLuint m_Buffer;
	vector<GLuint64> m_TextureAddressess;
};