#include "GBuffer.h"

GBuffer::GBuffer()
{
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)SDL_GL_GetProcAddress("glFramebufferTexture");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)SDL_GL_GetProcAddress("glDrawBuffers");
}

GBuffer::~GBuffer()
{

}

bool GBuffer::Init(int width,int height)
{
	m_Width = width;
	m_Height = height;
	glGenFramebuffers(1,&m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,m_FBO);
	//create Gbuffers
	glEnable(GL_TEXTURE_2D);
	//create diffusemap
	glGenTextures(1,&m_Textures[GBUFFER_DIFFUSE_24]);
	glBindTexture(GL_TEXTURE_2D,m_Textures[GBUFFER_DIFFUSE_24]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,m_Width,m_Height,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_Textures[GBUFFER_DIFFUSE_24],0);
	if(!glIsTexture(m_Textures[GBUFFER_DIFFUSE_24]))
		return false;
	//create normalmap
	glGenTextures(1, &m_Textures[GBUFFER_NORMAL24]);
	glBindTexture(GL_TEXTURE_2D, m_Textures[GBUFFER_NORMAL24]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,m_Width,m_Height,0,GL_RGB,GL_FLOAT,0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_Textures[GBUFFER_NORMAL24], 0);
	if (!glIsTexture(m_Textures[GBUFFER_NORMAL24]))
		return false;
	//create Material id map
	glGenTextures(1, &m_Textures[GBUFFER_MATID8]);
	glBindTexture(GL_TEXTURE_2D, m_Textures[GBUFFER_MATID8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, m_Width, m_Height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Textures[GBUFFER_MATID8], 0);
	if (!glIsTexture(m_Textures[GBUFFER_MATID8]))
		return false;
	//Roughness map
	glGenTextures(1, &m_Textures[GBUFFER_REFLECTION24_ROUGHNESS8]);
	glBindTexture(GL_TEXTURE_2D, m_Textures[GBUFFER_REFLECTION24_ROUGHNESS8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_Textures[GBUFFER_REFLECTION24_ROUGHNESS8], 0);
	if (!glIsTexture(m_Textures[GBUFFER_REFLECTION24_ROUGHNESS8]))
		return false;
	//create depthmap
	glGenTextures(1,&m_Textures[GBUFFER_DEPTH32]);
	glBindTexture(GL_TEXTURE_2D,m_Textures[GBUFFER_DEPTH32]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,m_Width,m_Height,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,m_Textures[GBUFFER_DEPTH32],0);
	if(!glIsTexture(m_Textures[GBUFFER_DEPTH32]))
		return false;
	//create Lightmap
	glGenTextures(1,&m_Textures[GBUFFER_LIGHT24]);
	glBindTexture(GL_TEXTURE_2D,m_Textures[GBUFFER_LIGHT24]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_Textures[GBUFFER_LIGHT24], 0);
	if (!glIsTexture(m_Textures[GBUFFER_LIGHT24]))
		return false;

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, drawBuffers);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FBO error!!, status: 0x%x\n",status);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return true;
}

void  GBuffer::ApplyGeometryStage()
{
		glClearColor(0.0f,0.0f,0.0f,0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_FBO);
		GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0,
								GL_COLOR_ATTACHMENT1,
								GL_COLOR_ATTACHMENT2,
								GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, drawBuffers);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
}

void  GBuffer::ApplyLightingStage()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT4};
	glDrawBuffers(1, drawBuffers);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::ApplyPostProcessStage()
{
	glBindFramebuffer(GL_FRAMEBUFFER , 0);
	//GLenum drawBuffers[] = { GL_NONE };
	//glDrawBuffers(1, drawBuffers);
	glClear(GL_COLOR_BUFFER_BIT);
}


void GBuffer::ApplyTexture(GLint unifromLocation, int index, GBUFFER_TYPE type)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_Textures[type]);
	glUniform1i(unifromLocation, index);
}

GLuint GBuffer::GetHandle()
{
	return m_FBO;
}

GLuint GBuffer::GetTextureHandle(GBUFFER_TYPE bufferID)
{
	return m_Textures[bufferID];
}