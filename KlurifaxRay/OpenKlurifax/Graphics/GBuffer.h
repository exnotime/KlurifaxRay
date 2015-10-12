#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
enum GBUFFER_TYPE
{
	GBUFFER_DIFFUSE_24,
	GBUFFER_NORMAL24,
	GBUFFER_MATID8,
	GBUFFER_REFLECTION24_ROUGHNESS8,
	GBUFFER_DEPTH32,
	GBUFFER_LIGHT24,
	GBUFFER_NUMBER_OF_BUFFERS
};

class GBuffer
{
private:
	int m_Width,m_Height;
	GLuint m_FBO;
	GLuint m_Textures[GBUFFER_NUMBER_OF_BUFFERS];
public:
	GBuffer();
	~GBuffer();
	bool Init(int width,int height);
	void ApplyGeometryStage();
	void ApplyLightingStage();
	void ApplyPostProcessStage();
	void ApplyTexture(GLint unifromLocation, int index, GBUFFER_TYPE type);
	GLuint GetHandle();
	GLuint GetTextureHandle(GBUFFER_TYPE bufferID);
};

