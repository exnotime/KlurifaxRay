#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

#define MaxNrOfParticles 1000

enum ParticleType{
	Emitter,
	SHELL
};
struct Particle
{
	float Type;
	glm::vec3 Position;
	glm::vec3 Velocity;
	float LifeTime;
};

class ParticleSystem{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init(glm::vec3 pos);
	void Update(float deltaTime);
	void Render();
private:
	bool m_IsFirst;
	int m_CurrTFB;
	int m_CurrVB;
	GLuint m_ParticleBuffers[2];
	GLuint m_TransformFeedback[2];
	Texture* m_Texture; //TODO: modify so that more than one texture is possible
	GLuint m_RandomTexture; //Texture handle to a 1D texture with random numbers between -1 -> 1
	unsigned int m_Time;
	ShaderProgram m_UpdateProgram;
	ShaderProgram m_RenderProgram;

};