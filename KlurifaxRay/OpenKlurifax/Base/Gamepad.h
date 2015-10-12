#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <string>

using std::string;

class Gamepad {
public:
	Gamepad(SDL_GameController* controller);
	~Gamepad();

	void Update();

	string GetName		( ) const;

	bool ButtonDown		( SDL_GameControllerButton button ) const;
	bool ButtonUpDown	( SDL_GameControllerButton button ) const;
	bool ButtonDownUp	( SDL_GameControllerButton button ) const;
	bool ButtonUp		( SDL_GameControllerButton button ) const;

	float RightStickX	( ) const;
	float RightStickY	( ) const;
	float LeftStickX	( ) const;
	float LeftStickY	( ) const;
	float LeftTrigger	( ) const;
	float RightTrigger	( ) const;

private:
	SDL_GameController* m_controller;

	Uint32	m_buttonsMask = 0;
	Uint32	m_prevButtonsMask = 0;
	bool 	m_connected = false;

	float m_rightStickX;
	float m_rightStickY;
	float m_leftStickX;
	float m_leftStickY;
	float m_leftTrigger;
	float m_rightTrigger;
};
