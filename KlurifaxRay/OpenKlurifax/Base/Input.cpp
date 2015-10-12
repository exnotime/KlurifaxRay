#include "Input.h"
#include <SDL2/SDL_events.h>
#include <cstring>
#include <string>

using std::memcpy;

Input* Input::GetInstance() {
	static Input input;
	return &input;
}

Input::~Input() {
	delete m_prevKeys;
	for (auto* c : m_controllers) {
		if (c) { 
			delete c;
		}
	}
}

void Input::Initialize() {
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_PumpEvents( );
	m_currentKeys = SDL_GetKeyboardState( &m_keyArrayLength );
	m_prevKeys = new Uint8[m_keyArrayLength];

	// Make room for 16 gamepads, windows is probably what causes this limit
	m_controllers.resize( 16 );
}

void Input::Update( ) {
	// Store previous keys for updown downup functionality
	memcpy(m_prevKeys, m_currentKeys, m_keyArrayLength);
	// Update input states
	SDL_PumpEvents( );
	m_currentKeys = SDL_GetKeyboardState( &m_keyArrayLength );
	
	// Track change in mouse position
	SDL_GetRelativeMouseState( &m_mouseDeltaX, &m_mouseDeltaY );
	// Store previous button mask for updown downup functionality
	m_prevMouseMask = m_currentMouseMask;
	// Get current mouse button mask
	m_currentMouseMask = SDL_GetMouseState( &m_mousePosX, &m_mousePosY );

	for (vector<Gamepad*>::iterator it = m_controllers.begin(); it != m_controllers.end(); ++it) {
		if (*it != nullptr) {
			(*it)->Update();
		}
	}
}

void Input::HandleEvents(SDL_Event& event) {
	switch (event.type) {
		case SDL_CONTROLLERDEVICEREMOVED: {
			Gamepad* gp = m_controllers.at( event.cdevice.which );
			if (gp != nullptr) {
				//Logger::Log(gp->GetName() + " " + std::to_string(event.cdevice.which) + " removed", "Gamepad", LogSeverity::INFO);
				m_controllers.at( event.cdevice.which ) = nullptr;
				delete gp;
			}
			break;
		}
		case SDL_CONTROLLERDEVICEADDED: {
			SDL_GameController* controller = nullptr;
			// Open controller so we can use it
			controller = SDL_GameControllerOpen( event.cdevice.which );
			if (controller) {
				m_controllers.at( event.cdevice.which ) = new Gamepad(controller);
				//Logger::Log(string(SDL_GameControllerName( controller )) + " " + std::to_string(event.cdevice.which) + " added",
				//	   	"Gamepad", LogSeverity::INFO);
			} else {
				//Logger::Log("Could not open gamecontroller " + std::to_string(event.cdevice.which)
					//+ ": " + SDL_GetError(), "Gamepad", LogSeverity::ERROR_);
			}
			break;
		}
	}
}

bool Input::KeyUpDown(SDL_Scancode key) const {
	return !m_prevKeys[key] && m_currentKeys[key];
}

bool Input::KeyDownUp(SDL_Scancode key) const {
	return m_prevKeys[key] && !m_currentKeys[key];
}

bool Input::KeyDown(SDL_Scancode key) const {
	return m_currentKeys[key] != 0;
}

bool Input::KeyUp(SDL_Scancode key) const {
	return !m_currentKeys[key];
}

bool Input::MouseUpDown(unsigned int button) const {
	return 	!(m_prevMouseMask & SDL_BUTTON(button))	&&	// prev up
			(m_currentMouseMask & SDL_BUTTON(button));	// current down
}

bool Input::MouseDownUp(unsigned int button) const {
	return 	(m_prevMouseMask & SDL_BUTTON(button))	&&	// prev down
			!(m_currentMouseMask & SDL_BUTTON(button));	// current up
}

bool Input::MouseDown(unsigned int button) const {
	return (m_currentMouseMask & SDL_BUTTON(button)) != 0;
}

bool Input::MouseUp(unsigned int button) const {
	return !(m_currentMouseMask & SDL_BUTTON(button));
}

int Input::MouseDeltaX() const {
	return m_mouseDeltaX;
}

int Input::MouseDeltaY() const {
	return m_mouseDeltaY;
}

int Input::MousePosX() const {
	return m_mousePosX;
}

int Input::MousePosY() const {
	return m_mousePosY;
}

Gamepad* Input::GetGamepad(unsigned int index) const {
	if (index >= m_controllers.size()) {
		return nullptr; 
	}
	return m_controllers.at( index );
}
