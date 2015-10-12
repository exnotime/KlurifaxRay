#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(){

}

TextureBuffer::~TextureBuffer(){
	glDeleteBuffers(1, &m_Buffer);
}

void TextureBuffer::Init(){
	glGenBuffers(1,&m_Buffer);
	m_TextureAddressess.reserve(MaxNrOfTextures);
}

void TextureBuffer::Build(){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint64)* MaxNrOfTextures, m_TextureAddressess.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TextureBuffer::Bind(GLuint program, GLuint bindingPoint){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_Buffer, 0, sizeof(GLuint64)* MaxNrOfTextures);
	GLuint bi = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "SamplerBuffer");
	glShaderStorageBlockBinding(program, bi, bindingPoint);
	//glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer);
}

void TextureBuffer::AddTextures(map<TextureHandle, Texture>& textures){
	for (auto& it : textures){
		m_TextureAddressess.push_back(it.second.GetAddress());
	}
}