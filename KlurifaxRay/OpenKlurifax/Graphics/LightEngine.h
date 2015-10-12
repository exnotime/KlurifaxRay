#pragma once
#define MaxNrOfPointlights 200
#define MaxNrOfDirlights 5
#define MaxNrOfSpotlights 50
#include "Lights.h"
#include <GL/glew.h>
#include <vector>
using std::vector;
#define g_LightEngine LightEngine::GetInstance()
class LightEngine{
public:
	~LightEngine();
	static LightEngine& GetInstance();
	PointLight* CreateDynamicPointLight();
	DirLight* CreateDynamicDirLight();
	SpotLight* CreateDynamicSpotLight();
	void CreateStaticPointLight(PointLight& light);
	void CreateStaticDirLight(DirLight& light);
	void CreateStaticSpotLight(SpotLight& light);
	void BuildBuffers();
	void UpdateBuffers();
	GLuint GetPointLightBuffer();
	GLuint GetDirLightBuffer();
	GLuint GetSpotLightBuffer();
	int GetNrOfPointLights();
	int GetNrOfDirLights();
	int GetNrOfSpotLights();
	void Clear();
private:
	LightEngine();

	vector<PointLight> m_PointLightBuffer;
	vector<DirLight> m_DirLightBuffer;
	vector<SpotLight> m_SpotLightBuffer;
	GLuint m_PointLightBufferHandle;
	GLuint m_DirLightBufferHandle;
	GLuint m_SpotLightBufferHandle;

	unsigned int m_NrOfPointLights;
	unsigned int m_NrOfDirLights;
	unsigned int m_NrOfSpotLights;
};