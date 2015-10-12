#include "LightEngine.h"

LightEngine::LightEngine(){
	m_NrOfPointLights = 0;
	m_NrOfDirLights = 0;
	m_NrOfSpotLights = 0;
	m_PointLightBuffer.reserve(MaxNrOfPointlights);
	m_DirLightBuffer.reserve(MaxNrOfDirlights);
	m_SpotLightBuffer.reserve(MaxNrOfSpotlights);
}

LightEngine::~LightEngine(){
	Clear();
}

LightEngine& LightEngine::GetInstance(){
	static LightEngine m_Engine;
	return m_Engine;
}

PointLight* LightEngine::CreateDynamicPointLight(){
	if (m_NrOfPointLights < MaxNrOfPointlights){
		PointLight light;
		m_PointLightBuffer.push_back(light);
		m_NrOfPointLights++;
		return &m_PointLightBuffer[m_PointLightBuffer.size() - 1];
	}
	else{
		printf("Maximum number of dynamic PointLights have been met, increase buffer size or delete old lights\n");
		return nullptr;
	}
}

DirLight* LightEngine::CreateDynamicDirLight(){
	if (m_NrOfDirLights < MaxNrOfDirlights){
		DirLight light;
		m_DirLightBuffer.push_back(light);
		m_NrOfDirLights++;
		return &m_DirLightBuffer[m_DirLightBuffer.size() - 1];
	}
	else{
		printf("Maximum number of dynamic DirLights have been met increase buffer size or delete old lights\n");
		return nullptr;
	}
}

SpotLight* LightEngine::CreateDynamicSpotLight(){
	if (m_NrOfSpotLights < MaxNrOfSpotlights){
		SpotLight light;
		m_SpotLightBuffer.push_back(light);
		m_NrOfSpotLights++;
		return &m_SpotLightBuffer[m_SpotLightBuffer.size() - 1];
	}
	else{
		printf("Maximum number of dynamic DirLights have been met increase buffer size or delete old lights\n");
		return nullptr;
	}
}

void LightEngine::CreateStaticPointLight(PointLight& light){
	if (m_NrOfPointLights < MaxNrOfPointlights){
		m_PointLightBuffer.push_back(light);
		m_NrOfPointLights++;
	}
}

void LightEngine::CreateStaticDirLight(DirLight& light){
	if (m_NrOfDirLights < MaxNrOfDirlights){
		m_DirLightBuffer.push_back(light);
		m_NrOfDirLights++;
	}
}

void LightEngine::CreateStaticSpotLight(SpotLight& light){
	if (m_NrOfSpotLights < MaxNrOfSpotlights){
		m_SpotLightBuffer.push_back(light);
		m_NrOfSpotLights++;
	}
}

void LightEngine::BuildBuffers(){

	glGenBuffers(1, &m_PointLightBufferHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_PointLightBufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, m_PointLightBuffer.capacity() * sizeof(PointLight), m_PointLightBuffer.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_DirLightBufferHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_DirLightBufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, m_DirLightBuffer.capacity() * sizeof(DirLight), m_DirLightBuffer.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_SpotLightBufferHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_SpotLightBufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, m_SpotLightBuffer.capacity() * sizeof(SpotLight), m_SpotLightBuffer.data(), GL_DYNAMIC_DRAW);
}

void LightEngine::UpdateBuffers(){
	glBindBuffer(GL_UNIFORM_BUFFER, m_PointLightBufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_PointLightBuffer.size() * sizeof(PointLight), m_PointLightBuffer.data());

	glBindBuffer(GL_UNIFORM_BUFFER, m_DirLightBufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_DirLightBuffer.size() * sizeof(DirLight), m_DirLightBuffer.data());

	glBindBuffer(GL_UNIFORM_BUFFER, m_SpotLightBufferHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_SpotLightBuffer.size() * sizeof(SpotLight), m_SpotLightBuffer.data());
}

GLuint LightEngine::GetPointLightBuffer(){
	return m_PointLightBufferHandle;
}

GLuint LightEngine::GetDirLightBuffer(){
	return m_DirLightBufferHandle;
}

GLuint LightEngine::GetSpotLightBuffer(){
	return m_SpotLightBufferHandle;
}

int LightEngine::GetNrOfDirLights(){
	return m_NrOfDirLights;
}

int LightEngine::GetNrOfPointLights(){
	return m_NrOfPointLights;
}

int LightEngine::GetNrOfSpotLights(){
	return m_NrOfSpotLights;
}

void LightEngine::Clear(){
	//delete all buffers
	m_DirLightBuffer.clear();
	m_PointLightBuffer.clear();
	m_SpotLightBuffer.clear();
	glDeleteBuffers(1, &m_DirLightBufferHandle);
	glDeleteBuffers(1, &m_SpotLightBufferHandle);
	glDeleteBuffers(1, &m_PointLightBufferHandle);
}
