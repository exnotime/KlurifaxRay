#include "Engine.h"
Engine::Engine()
{
	m_Graphics = nullptr;
	m_Game = nullptr;
	m_Timer = nullptr;
	m_FPSTimer = nullptr;
	m_RenderQueue = nullptr;
	m_EventSystem = nullptr;
}

Engine::~Engine()
{
	if (m_Graphics) delete m_Graphics;
	if (m_Game) delete m_Game;
	if (m_Timer) delete m_Timer;
	if (m_FPSTimer) delete m_FPSTimer;
	if (m_RenderQueue) delete m_RenderQueue;
}

void Engine::Init(int ScreenWidth, int ScreenHeight, bool Fullscreen, bool vsync)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m_Graphics = new GraphicsEngine();
	m_RenderQueue = new RenderQueue();
	//m_SoundEngine = new SoundEngine();
	//m_SoundEngine->Init();
	
	m_Graphics->Init(ScreenWidth, ScreenHeight, Fullscreen,vsync, true);
	m_Game = new Game();
	m_Game->Init(&g_LightEngine);
	m_Timer = new Timer();
	m_Timer->Start();
	m_FPSTimer = new Timer();
	m_FrameCounter = 0;
	m_FPSTimer->Start();
	m_Timer->Start();
	m_EventSystem = EventManager::GetInstance();
	g_ModelBank.BuildBuffers();
	g_MaterialBank.BuildMaterialBuffer();
	g_LightEngine.BuildBuffers();
	m_Graphics->BindBuffers();
}

bool Engine::Run()
{
	m_Graphics->Draw(m_RenderQueue, &g_LightEngine);
	m_RenderQueue->Clear();
	if (m_Game->Update(static_cast<float>(m_Timer->DeltaTime()))){
		return false;
	}
	g_LightEngine.UpdateBuffers();
	m_Game->Draw(m_RenderQueue);
	
	//m_SoundEngine->Update();
	if (SDL_PollEvent(&m_Event) > 0){
		if (m_Event.type == SDL_KEYDOWN){
			if (m_Event.key.keysym.sym == SDLK_ESCAPE){
				SDL_Quit();
				return false;
			}
		}
		if (m_Event.type == SDL_QUIT){
			SDL_Quit();
			return false;
		}
	}
	m_EventSystem->ClearEvents();
	if (m_FPSTimer->Time() >= 1.0f){
		string windowName = string("OpenKlurifax : FPS: ");
		std::stringstream ss;
		ss << windowName;
		ss << m_FrameCounter;
		ss << " Frametime : ";
		ss << m_FPSTimer->Time() / m_FrameCounter;
		SDL_SetWindowTitle(m_Graphics->GetWindow(), ss.str().c_str());
		m_FrameCounter = 0;
		m_FPSTimer->Restart();
	}
	m_FrameCounter++;
	
	return true;
}