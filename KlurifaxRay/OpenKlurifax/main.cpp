#include "Base/Engine.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <SDL2/SDL_main.h>

int main(int argc, char *argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine* GameEngine = new Engine();
	GameEngine->Init(640,480, false, true);
	while(GameEngine->Run())
	{

	}
	delete GameEngine;
	return 0;
}