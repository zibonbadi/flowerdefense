#include "keymapper.hh"

KeyMapper g_keymapper;

int KeyMapper::bind(SDL_Keycode key, Event* event){
	try{
		//this->key_events.at(key) = event;
		this->key_events.insert(std::pair<SDL_Keycode, Event*>(key,event));
		ENGINE_DEBUG_MSG("KeyMapper.bind(" << key << "): Bound key " << key << " to \"" << this->key_events.at(key)->get("type") << "\".");
		return 0;
	}catch(std::exception &e){
		std::cerr << "KeyMapper.bind(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::unbind(SDL_Keycode key){
	try{
		this->key_events.erase(key);
		ENGINE_DEBUG_MSG("KeyMapper.unbind(" << key << "): Unbound key " << key);
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.unbind(" << key << ") exception: " << e.what() << std::endl;
		return -1;
	}
};

int KeyMapper::probe(SDL_KeyboardEvent sdlevent){
	try{
		Event* e_send = this->key_events.at(sdlevent.keysym.sym);
		// status_edge: "up" "down" "on" "off" "toggle"
		if(sdlevent.repeat == 0){
			switch(sdlevent.type){
			case SDL_KEYDOWN:{
				e_send->set("status_edge", "down");
				break;
			}
			case SDL_KEYUP:{
				e_send->set("status_edge", "up");
				break;
			}
			};
			g_eventbus.send(e_send);
			std::cerr << "KeyMapper.probe(" << sdlevent.keysym.sym << "): Called. \"" << e_send->get("status_edge") << "\"" << std::endl;
		};
		return 0;
	}catch (std::exception &e){
		std::cerr << "KeyMapper.probe() exception: " << e.what() << std::endl;
		return -1;
	}
};
