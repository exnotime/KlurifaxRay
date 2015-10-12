#include "GraphicsEngine.h"
#include "../Base/ModelBank.h"
#include "../Base/MaterialBank.h"
#define PI 3.14159265359f
GraphicsEngine::GraphicsEngine()
{
	m_SDLWindow = nullptr;
	m_GBuffer = nullptr;
}

GraphicsEngine::~GraphicsEngine()
{
	SDL_DestroyWindow(m_SDLWindow);
	if(m_GBuffer) delete m_GBuffer;
	glDeleteTextures(1, &m_ComputeTexture);
}

SDL_Window* GraphicsEngine::GetWindow(){
	return m_SDLWindow;
}

bool GraphicsEngine::Init(int ScreenWidth,int ScreenHeight,bool Fullscreen,bool vsync,bool Deffered)
{
	m_DefferedFlag = Deffered;
	m_MSAA = 1.0f;
	m_ScreenWidth = ScreenWidth * m_MSAA;
	m_ScreenHeight = ScreenHeight* m_MSAA;
	m_Fullscreen = Fullscreen;
	m_VSync = vsync;
	if (Fullscreen){
		m_SDLWindow = SDL_CreateWindow("OpenKlurifax", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ScreenWidth, ScreenHeight, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	} else {
		m_SDLWindow = SDL_CreateWindow("OpenKlurifax", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	SDL_GL_CreateContext(m_SDLWindow);
	if (vsync){
		SDL_GL_SetSwapInterval(1);
	}
	else {
		SDL_GL_SetSwapInterval(0);
	}
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	printf("OpenGL version %s\n",(char*)glGetString(GL_VERSION));

	//char* ext = (char*)glGetString(GL_EXTENSIONS);
	//if(m_DefferedFlag)
	//{
	//	m_GBuffer = new GBuffer();
	//	if (!m_GBuffer->Init(ScreenWidth * m_MSAA, ScreenHeight * m_MSAA))
	//		return false;
	////}
	//m_DefRenderProgram.Init();
	//shaders
	//m_DeferedLightProgram.LoadCompleteShaderProgramFromFile("Graphics/Shaders/DeferedLighting.glsl", true);
	m_DeferedPostProcessProgram.LoadCompleteShaderProgramFromFile("Graphics/Shaders/DeferedPostProcess.glsl",true);
	m_PrimaryRayProgram.LoadCompleteShaderProgramFromFile("Graphics/Shaders/PrimaryRays.glsl", true);
	m_SecondaryRayProgram.LoadCompleteShaderProgramFromFile("Graphics/Shaders/SecondaryRays.glsl", true);
	glGenTextures(1,&m_ComputeTexture);
	glBindTexture(GL_TEXTURE_2D, m_ComputeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ScreenWidth, m_ScreenHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	m_RayBuffer.Init(m_ScreenWidth, m_ScreenHeight);
	m_PrimBuffer.Init();

	Sphere s;
	s.c = vec3(10, 0, 0);
	s.r = 2.0f;
	s.Color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	
	s.c = vec3(0, -9, -3);
	s.r = 1.0f;
	s.Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	s.c = vec3(13, 2, 0);
	s.r = 4.0f;
	s.Color = vec4(0.2f, 0.5f, 1.0f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	s.c = vec3(0, -4, -8);
	s.r = 3.0f;
	s.Color = vec4(0.3f, 1.0f, 0.1f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	s.c = vec3(3, 1, 0);
	s.r = 1.0f;
	s.Color = vec4(0.0f, 0.5f, 1.0f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	s.c = vec3(-13, -13, 0);
	s.r = 7.0f;
	s.Color = vec4(1.0f, 0.2f, 1.0f, 1.0f);
	m_PrimBuffer.AddSphere(s);
	
	Triangle t;
	t.p0 = vec4(0, 10, 0, 0);
	t.p1 = vec4(10, 1, 7, 0);
	t.p2 = vec4(10, 0, 0, 0);
	t.Color = vec4(0, 0.5f, 1.0f, 1.0f);
	m_PrimBuffer.AddTriangle(t);
	
	m_PrimBuffer.GenerateAABB(vec3(-30), vec3(30));
	m_PrimBuffer.Build();
	m_PrimBuffer.Bind(m_SecondaryRayProgram.GetHandle(), 3);
	m_ModelBuffer.Init();
	m_TextureBuffer.Init();

	m_RayBuffer.Apply(m_PrimaryRayProgram.GetHandle());
	return true;
}

void GraphicsEngine::ReInit(int ScreenWidth, int ScreenHeight){
	//Reinit raybuffer and frame texture
	m_ScreenWidth = ScreenWidth * m_MSAA;
	m_ScreenHeight = ScreenHeight* m_MSAA;
	glBindTexture(GL_TEXTURE_2D, m_ComputeTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ScreenWidth, m_ScreenHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_RayBuffer.Init(m_ScreenWidth, m_ScreenHeight);
	m_RayBuffer.Apply(m_PrimaryRayProgram.GetHandle());
}
int GraphicsEngine::Draw(RenderQueue* rq,LightEngine* le)
{
	if (rq->GetScenes()->size() == 0){
		return 0;
	}
	RenderScene scene = rq->GetScenes()->at(0);
	Camera cam = *scene.Camera;
	OGLTimer timer;
	timer.Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Fill ray buffer
	m_PrimaryRayProgram.Apply();
	m_PrimaryRayProgram.SetUniformValue("camPos", cam.GetPos());
	m_PrimaryRayProgram.SetUniformValue("camUp", cam.GetUp());
	m_PrimaryRayProgram.SetUniformValue("camRight", cam.GetRight());
	vec3 camDir = (-static_cast<float>(m_ScreenWidth * 0.5f)) * cam.GetRight() + (static_cast<float>(m_ScreenHeight * 0.5f) * cam.GetUp()) - (static_cast<float>(m_ScreenHeight * 0.5f) / tan(cam.GetFOV() / 360.0f * PI)) * cam.GetLook();
	m_PrimaryRayProgram.SetUniformValue("camDir", camDir);
	m_PrimaryRayProgram.SetUniformValue("width", static_cast<float>(m_ScreenWidth));
	
	glDispatchCompute(m_ScreenWidth / 16, m_ScreenHeight / 16, 1);
	//write to image
	static float brightness = 1.0f;
	if (Input::GetInstance()->KeyDown(SDL_SCANCODE_B)){
		brightness -= 0.01f;
	}
	int BounceCount = 1;
	for (int i = 0; i < BounceCount; ++i){
		m_SecondaryRayProgram.Apply();
		glBindImageTexture(0, m_ComputeTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
		m_RayBuffer.Apply(m_SecondaryRayProgram.GetHandle());
		m_SecondaryRayProgram.SetUniformTexture("CubeTex", *g_MaterialBank.FetchTexture(1));
		m_SecondaryRayProgram.SetUniformValue("width", static_cast<float>(m_ScreenWidth));
		m_SecondaryRayProgram.SetUniformValue("materialIndex", 0);
		m_SecondaryRayProgram.SetUniformValue("brightness", brightness);
		m_SecondaryRayProgram.SetUniformValue("NrOfSpheres", m_PrimBuffer.nSpheres());
		m_SecondaryRayProgram.SetUniformValue("NrOfTriangles", m_PrimBuffer.nTriangles());
		m_SecondaryRayProgram.SetUniformValue("NrOfModels", (int)g_ModelBank.GetModels().size());
		m_SecondaryRayProgram.SetUniformValue("iteration", i + 1);
		m_SecondaryRayProgram.SetUniformValue("NrOfPointlights", g_LightEngine.GetNrOfPointLights());
		m_SecondaryRayProgram.SetUniformValue("NrOfDirlights", g_LightEngine.GetNrOfDirLights());
		glDispatchCompute(m_ScreenWidth / 8, m_ScreenHeight / 8, 1);
		//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);//wait for it to finish
	}
	//draw image to framebuffer
	m_DeferedPostProcessProgram.Apply();
	glViewport(0, 0, m_ScreenWidth / m_MSAA, m_ScreenHeight / m_MSAA);
	//bind texture
	GLint loc = glGetUniformLocation(m_DeferedPostProcessProgram.GetHandle(), "InTex");
	glUniform1i(loc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ComputeTexture);
	glDrawArrays(GL_POINTS, 0, 1);
	//swap for a new frame and wait for vsync
	SDL_GL_SwapWindow(m_SDLWindow);
	return timer.Stop();
}

void GraphicsEngine::BindBuffers(){
	//this function must be called after all models have been loaded
	GLuint bi = glGetProgramResourceIndex(m_SecondaryRayProgram.GetHandle(), GL_UNIFORM_BLOCK, "MaterialBuffer");
	glUniformBlockBinding(m_SecondaryRayProgram.GetHandle(), bi, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_MaterialBank.GetMaterialBuffer());

	bi = glGetUniformBlockIndex(m_SecondaryRayProgram.GetHandle(), "PointLightBuffer");
	glUniformBlockBinding(m_SecondaryRayProgram.GetHandle(), bi, 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, g_LightEngine.GetPointLightBuffer());

	bi = glGetUniformBlockIndex(m_SecondaryRayProgram.GetHandle(), "DirLightBuffer");
	glUniformBlockBinding(m_SecondaryRayProgram.GetHandle(), bi, 4);
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, g_LightEngine.GetDirLightBuffer());

	m_ModelBuffer.AddModels(g_ModelBank.GetModels(), g_ModelBank.GetVertices(), g_ModelBank.GetIndices());
	m_ModelBuffer.Build();
	m_ModelBuffer.Bind(m_SecondaryRayProgram.GetHandle(), 8);

	m_TextureBuffer.AddTextures(g_MaterialBank.GetTextures());
	m_TextureBuffer.Build();
	m_TextureBuffer.Bind(m_SecondaryRayProgram.GetHandle(),5);
}