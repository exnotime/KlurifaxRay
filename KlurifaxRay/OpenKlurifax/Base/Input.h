#pragma once

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>
#include <vector>
#include "Gamepad.h"

using std::vector;

class Input {
public:
	static Input* GetInstance();

	~Input	();

	void 	Initialize	( );
	void 	Update		( );
	void 	HandleEvents( SDL_Event& event );

	bool 	KeyUpDown	( SDL_Scancode key ) const;
	bool 	KeyDownUp	( SDL_Scancode key ) const;
	bool 	KeyDown		( SDL_Scancode key ) const;
	bool 	KeyUp		( SDL_Scancode key ) const;

	bool 	MouseUpDown ( unsigned int button ) const;
	bool 	MouseDownUp ( unsigned int button ) const;
	bool 	MouseDown	( unsigned int button ) const;
	bool 	MouseUp		( unsigned int button ) const;

	int 	MouseDeltaX	( ) const;
	int 	MouseDeltaY	( ) const;
	int 	MousePosX	( ) const;
	int 	MousePosY	( ) const;
	
	Gamepad* GetGamepad ( unsigned int index ) const;
private:
	vector<Gamepad*> m_controllers;
	
	const Uint8* 	m_currentKeys 	= nullptr;
	Uint8* 			m_prevKeys 		= nullptr;

	int 	m_keyArrayLength = 0;
	int 	m_mouseDeltaX;
	int 	m_mouseDeltaY;
	int 	m_mousePosX;
	int 	m_mousePosY;
	Uint32	m_currentMouseMask;
	Uint32	m_prevMouseMask;
};
