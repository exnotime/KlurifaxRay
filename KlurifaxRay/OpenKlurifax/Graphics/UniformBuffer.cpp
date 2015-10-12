#include "UniformBuffer.h"
UniformBuffer::UniformBuffer()
{
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::Init(void* data, int size, GLuint shaderProgram, const char* blockname)
{
	glGenBuffers(1, &m_Handle);
	if (!glIsBuffer(m_Handle)){
		printf("problem creating uniform buffer\n");
		return;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);

	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, blockname);
	GLuint bindingPoint = 0;
	glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);
}

void UniformBuffer::Update(int offset, int size, void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
void UniformBuffer::Apply(GLuint BindingPoint, GLuint BindingIndex)
{
	
}
