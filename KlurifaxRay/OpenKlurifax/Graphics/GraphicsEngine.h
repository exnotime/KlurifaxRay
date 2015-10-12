#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "GBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "DeferedRenderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "RenderQueue.h"
#include "LightEngine.h"
#include "../Base/MaterialBank.h"
#include "../Base/ModelBank.h"
#include "../Base/Input.h"
#include "../Base/Timer.h"
#include "OGLTimer.h"
#include "Rays/RayBuffer.h"
#include "Rays/PrimitivesBuffer.h"
#include "Rays/ModelBuffer.h"
#include "Rays/TextureBuffer.h"
#include <vector>

using std::vector;
class GraphicsEngine
{
private:
	SDL_Window* m_SDLWindow;
	GBuffer* m_GBuffer;
	int m_ScreenWidth;
	int m_ScreenHeight;
	bool m_DefferedFlag;
	bool m_VSync;
	bool m_Fullscreen;
	float m_MSAA;
	DeferedRenderProgram m_DefRenderProgram; //Default program
	MaterialBank* m_MaterialBank;
	ModelBank* m_ModelBank;
	//shaderprograms
	ShaderProgram m_DeferedPostProcessProgram;
	RayBuffer m_RayBuffer;
	ShaderProgram m_ComputeProgram;
	ShaderProgram m_PrimaryRayProgram;
	ShaderProgram m_SecondaryRayProgram;
	GLuint m_ComputeTexture;
	ModelBuffer m_ModelBuffer;
	PrimitivesBuffer m_PrimBuffer;
	TextureBuffer	m_TextureBuffer;
public:
	GraphicsEngine();
	~GraphicsEngine();
	bool Init(int ScreenWidth,int ScreenHeight,bool Fullscreen,bool vsync,bool Deffered); //run this to set up the engine
	void InitSettings(int screenWidth, int screenHeight, bool vsync, bool fullscreen); //inits settings, run this to change resolution, vsync etc
	int Draw(RenderQueue* rq, LightEngine* le);
	void ReInit(int ScreenWidth, int ScreenHeight);
	SDL_Window* GetWindow();
	void BindBuffers();
};