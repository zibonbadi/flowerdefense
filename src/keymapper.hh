#ifndef KEYMAPPER_H
#define KEYMAPPER_H

#include "headers.hh"

class KeyMapper{
	private:
		std::map<SDL_Keycode*, Event> callbacks;
		Eventbus* target;

	public:
		KeyMapper(Eventbus* target);
		~KeyMapper();
		
		// Int types for error codes
		int bind(SDL_Keycode key, Event event);
		int unbind(SDL_Keycode key);
		int probe(SDL_KeyboardEvent key);
};
#endif

