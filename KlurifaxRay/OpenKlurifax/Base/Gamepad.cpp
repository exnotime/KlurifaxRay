#include "Gamepad.h"
#include <iostream>

// For making axises in the interval [-1,1]
#define GAMEPAD_AXIS_FACTOR 1 / 32768.0f

using std::endl;
using std::cout;

Gamepad::Gamepad(SDL_GameController* controller)
: m_controller(controller) {
	
}

Gamepad::~Gamepad() {
	//SDL_GameControllerClose( m_controller );
	//delete m_controller;
}

void Gamepad::Update() {
	// Only update if controller is attached
	if ( SDL_GameControllerGetAttached( m_controller ) == SDL_TRUE ) {
		// Store previous mask for updown downup functionality
		m_prevButtonsMask = m_buttonsMask;
		// Reset mask to no buttons being down
		m_buttonsMask = 0;
		// Loop through all SDL controller buttons
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
			// Set bit corresponding to the specific button
			m_buttonsMask |=
				SDL_GameControllerGetButton( m_controller, static_cast<SDL_GameControllerButton>( i ) ) << i;
		}

		// Axis													   Axis									Make [-1,1]
		m_rightStickX 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_RIGHTX 		) * GAMEPAD_AXIS_FACTOR;
		m_rightStickY 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_RIGHTY 		) * GAMEPAD_AXIS_FACTOR;
		m_leftStickX 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_LEFTX 		) * GAMEPAD_AXIS_FACTOR;
		m_leftStickY 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_LEFTY 		) * GAMEPAD_AXIS_FACTOR;
		m_leftTrigger 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT 	) * GAMEPAD_AXIS_FACTOR;
		m_rightTrigger 	= SDL_GameControllerGetAxis( m_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT ) * GAMEPAD_AXIS_FACTOR;

		if ( m_connected == false ) {
			//Logger::Log(string(SDL_GameControllerName( m_controller )) + " connected", "Gamepad", LogSeverity::INFO);
			m_connected = true;
		}
	} else {
		m_connected = false;
	}
}

string Gamepad::GetName() const {
	return string(SDL_GameControllerName( m_controller ));
}

bool Gamepad::ButtonDown(SDL_GameControllerButton button) const {
	// Check the bit corresponding to the specified button
	return     (m_buttonsMask         & (1 << button)) != 0;
}

bool Gamepad::ButtonUpDown(SDL_GameControllerButton button) const {
	return 	!(m_prevButtonsMask	& (1 << button)) && // Previous up
			(m_buttonsMask 		& (1 << button)); 	// Current down
}

bool Gamepad::ButtonDownUp(SDL_GameControllerButton button) const {
	return 	(m_prevButtonsMask 	& (1 << button)) && // Previous down
			!(m_buttonsMask 	& (1 << button)); 	// Current up
}

bool Gamepad::ButtonUp(SDL_GameControllerButton button) const {
	// Check the bit corresponding to the specified button.
	// It shouldn't be set.
	return 	!(m_buttonsMask 	& (1 << button));
}

float Gamepad::RightStickX () const {
	return m_rightStickX;
}

float Gamepad::RightStickY () const {
	return m_rightStickY;
}

float Gamepad::LeftStickX () const {
	return m_leftStickX;
}

float Gamepad::LeftStickY () const {
	return m_leftStickY;
}

float Gamepad::LeftTrigger() const {
	return m_leftTrigger;
}

float Gamepad::RightTrigger() const {
	return m_rightTrigger;
}
