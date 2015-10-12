#include "RayBuffer.h"
#include <stdio.h>
RayBuffer::RayBuffer(){

}

RayBuffer::~RayBuffer(){

}

void RayBuffer::Init(int screenWidth, int screenHeight){
	glGenBuffers(1, &m_RayBuffer);
	if (m_RayBuffer <= 0){
		printf("could not create ray buffer\n");
		return;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RayBuffer);
	//1.4f is hack since for some reason the buffer didnt get large enough without it
	glBufferData(GL_SHADER_STORAGE_BUFFER, screenWidth * screenHeight * sizeof(Ray) * 1.4f, 0, GL_DYNAMIC_COPY);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, m_RayBuffer, 0, screenWidth * screenHeight * sizeof(Ray) * 1.4f );
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void RayBuffer::Apply(GLuint program){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RayBuffer);
	GLuint blockIndex = 0;
	blockIndex = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "RayBuffer");
	glShaderStorageBlockBinding(program, blockIndex, 2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}