#pragma once
#include "MaterialBank.h"
#include "ModelBank.h"
#include "../Graphics/RenderQueue.h"
#include "../Graphics/LightEngine.h"
#include "glm/glm.hpp"
#include "Input.h"
#include "../Graphics/Camera.h"
#include "Timer.h"
#include "EventManager.h"
using glm::mat4x4;
using glm::vec3;

#define GAME_MUSIC_NR_OF_BACKTRACKS 3

class Game
{
public:
	Game();
	~Game();
	void Init(LightEngine* le);
	bool Update(float deltaTime);
	void Draw(RenderQueue* rq);
private:
	Camera m_Cam;
	Timer m_Timer;
	mat4x4 world_plane;
	mat4x4 world; ///debug
	ModelHandle mamechi;
	ModelHandle miku;
	ModelHandle Lucina;
	ModelHandle plane;
	ModelHandle sword;
	ModelHandle scene;
	Input* input;
	PointLight* p;
	DirLight* dl;
};