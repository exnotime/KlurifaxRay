#pragma once
#include <sstream>
#include <SDL2/SDL.h>
#include "../Graphics/GraphicsEngine.h"
#include "../Graphics/RenderQueue.h"
#include "../Graphics/LightEngine.h"
#include "Game.h"
#include "Timer.h"
#include "EventManager.h"
class Engine
{
private:
	GraphicsEngine* m_Graphics;
	Game* m_Game;
	RenderQueue* m_RenderQueue;
	Timer* m_Timer;
	Timer* m_FPSTimer;
	int m_FrameCounter;
	SDL_Event m_Event;
	EventManager* m_EventSystem;
public:
	Engine();
	~Engine();
	bool Run();
	void Init(int ScreenWidth,int ScreenHeight,bool Fullscreen,bool vsync);
};
