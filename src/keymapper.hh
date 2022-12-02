#pragma once
#include "headers.hh"
		
// status_edge: "up" "down" "on" "off" "toggle"

class KeyMapper{
	private:
		std::map<SDL_Keycode, Event> key_events;

	public:		
		// Int types for error codes
		int bind(SDL_Keycode key, Event event);
		int unbind(SDL_Keycode key);
		int probe(SDL_KeyboardEvent key);
};

