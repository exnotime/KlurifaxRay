#pragma once
#include <GL/glew.h>
#include <stdio.h>
class UniformBuffer
{
public:
	UniformBuffer();
	~UniformBuffer();
	void Init(void* data, int size, GLuint shaderProgram, const char* blockname);
	void Update(int offset, int size, void* data);
	void Apply(GLuint BindingPoint, GLuint BindingIndex);
private:
	GLuint m_Handle;
};