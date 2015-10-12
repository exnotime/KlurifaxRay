#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
struct Ray
{
	glm::vec4 o;
	glm::vec4 d;
};

class RayBuffer{
public:
	RayBuffer();
	~RayBuffer();
	void Init(int screenWidth, int screenHeight);
	void Apply(GLuint program);
	GLuint GetRayBuffer();

private:
	GLuint m_RayBuffer;
};