#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
	m_Time = 0;
}

ParticleSystem::~ParticleSystem(){

}

void ParticleSystem::Init(glm::vec3 pos){
	Particle Particles[MaxNrOfParticles];
	memset(Particles, 0, sizeof(Particles));

	Particles[0].Type = Emitter;
	Particles[0].Position = pos;
	Particles[0].Velocity = glm::vec3(0.0f);
	Particles[0].LifeTime = 0.0f;

	glGenTransformFeedbacks(2, m_TransformFeedback);
	glGenBuffers(2, m_ParticleBuffers);
	for (int i = 0; i < 2; ++i){
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TransformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_ParticleBuffers[i]);
	}
	//Generate random texture
	const int width = 512, height = 512;
	float data[width * height];
	for (int i = 0; i < width * height; ++i){
		data[i] = rand() / static_cast<float>(RAND_MAX);
		data[i] = (data[i] * 2) - 1;
	}
	glGenTextures(1, &m_RandomTexture);
	glBindTexture(GL_TEXTURE_2D, m_RandomTexture);
	glTexImage1D(GL_TEXTURE_2D, 0, GL_RGBA32F, width * height, 0, GL_RGBA, GL_FLOAT, data);

	m_CurrVB = 0;
	m_CurrTFB = 1;
}

void ParticleSystem::Update(float deltaTime) {
	m_Time += static_cast<int>(deltaTime * 1000.0f);
	//TODO: Set up shader properly
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleBuffers[m_CurrVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TransformFeedback[m_CurrTFB]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); // type
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4); // position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16); // velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28); // lifetime

	glBeginTransformFeedback(GL_POINTS);
	if (m_IsFirst){
		glDrawArrays(GL_POINTS, 0, 1);
		m_IsFirst = false;
	}
	else{
		glDrawTransformFeedback(GL_POINTS, m_TransformFeedback[m_CurrTFB]);
	}
	glEndTransformFeedback();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void ParticleSystem::Render(){

}